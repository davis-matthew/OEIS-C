/*
 * A183538  Number of nX4 0..3 arrays with each element equal to either the
 *          sum mod 4 of its horizontal and vertical neighbors or the sum mod
 *          4 of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of n×4 arrays over {0,1,2,3} whose each entry equals
 *            either the sum mod 4 of its horizontal plus vertical neighbors
 *            or the sum mod 4 of its diagonal plus antidiagonal neighbors
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses dynamic programming over pairs of consecutive rows.
   Let R be the set of all 4‑digit rows (256 elements).  For a pair
   (p,c) of rows (previous, current) we precompute all rows r that can
   follow it, i.e. (p,c,r) satisfies the local rule.  Then a(n) is the
   number of length‑n paths starting at row 0 and ending at row 0.  */

/* Standard includes required by both C and C++ environments. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Include the helper that allocates an array of seq_t. */
#include "../utils/alloc_seq_array.h"   /* declares seq_t *alloc_seq_array(size_t); */
#include "../utils/free_seq_array.h"     /* declares void free_seq_array(seq_t *); */

/* --------------------------------------------------------------------- */
/*  Global data                                                   */
/* --------------------------------------------------------------------- */
static bool tables_ready = false;          /* guard */
static uint8_t  digits[256][4];            /* decomposed digits of a row */
static uint16_t *adj_len   = NULL;         /* number of c for each (a,b) */
static uint32_t *adj_start = NULL;         /* start index in adj_next */
static uint8_t  *adj_next  = NULL;         /* concatenated adjacency lists */
static bool     *allowed_zero = NULL;       /* true if transition to row 0 is OK */

/* --------------------------------------------------------------------- */
/*  Helper – what makes a triple (a,b,c) valid                   */
/* --------------------------------------------------------------------- */
static bool valid_transition(uint8_t a, uint8_t b, uint8_t c)
{
    /* digits of a, b, c are already decomposed in digits[][] */
    for (int j = 0; j < 4; ++j) {
        int bj  = digits[b][j];
        int bl  = (j > 0)     ? digits[b][j-1] : 0;
        int br  = (j < 3)     ? digits[b][j+1] : 0;
        int aj  = digits[a][j];
        int cj  = digits[c][j];

        /* horizontal + vertical sum */
        int sum_hv = (bl + br + aj + cj) & 3;

        /* diagonal + antidiagonal sum */
        int ad   = (j > 0)     ? digits[a][j-1] : 0;
        int cd   = (j < 3)     ? digits[c][j+1] : 0;
        int aa   = (j < 3)     ? digits[a][j+1] : 0;
        int ca   = (j > 0)     ? digits[c][j-1] : 0;
        int sum_d = (ad + cd + aa + ca) & 3;

        if (bj != sum_hv && bj != sum_d)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Build global tables for all 256×256 pairs of rows            */
/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    if (tables_ready)
        return;
    tables_ready = true;

    /* Pre‑decompose digits of each row once */
    for (int r = 0; r < 256; ++r)
        for (int j = 0; j < 4; ++j)
            digits[r][j] = (r >> (2*j)) & 3U;

    const size_t STATE_CNT = 256ULL * 256ULL;   /* 65536 */

    /* First pass: count edges per state */
    adj_len = (uint16_t*)malloc(STATE_CNT * sizeof(*adj_len));
    uint32_t total_edges = 0;
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            uint16_t cnt = 0;
            for (int c = 0; c < 256; ++c)
                if (valid_transition(a, (uint8_t)b, (uint8_t)c))
                    ++cnt;
            adj_len[(a<<8)|b] = cnt;
            total_edges += cnt;
        }
    }

    /* Second pass: record start indices and adjacency list */
    adj_start = (uint32_t*)malloc((STATE_CNT+1) * sizeof(*adj_start));
    adj_start[0] = 0;
    for (size_t i = 0; i < STATE_CNT; ++i)
        adj_start[i+1] = adj_start[i] + adj_len[i];

    adj_next = (uint8_t*)malloc(total_edges * sizeof(*adj_next));
    allowed_zero = (bool*)calloc(STATE_CNT, sizeof(*allowed_zero));

    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            size_t idx  = (a<<8)|b;
            size_t pos  = adj_start[idx];
            for (int c = 0; c < 256; ++c) {
                if (valid_transition(a, (uint8_t)b, (uint8_t)c)) {
                    adj_next[pos++] = (uint8_t)c;
                    if (c == 0)
                        allowed_zero[idx] = true;
                }
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; guard */
        return 0;

    init_tables();

    const size_t STATE_CNT = 256ULL * 256ULL;   /* 65536 */

    /* Allocate DP arrays using the provided helper. */
    seq_t *cur  = alloc_seq_array(STATE_CNT);
    seq_t *next = alloc_seq_array(STATE_CNT);
    if (!cur || !next)
        return 0;       /* allocation failure – unlikely in tests */

    /* Initialise counts for the first row: (prev=0, cur=row) */
    memset(cur, 0, STATE_CNT * sizeof(seq_t));
    for (int r1 = 0; r1 < 256; ++r1)
        cur[(0<<8)|r1] = 1;

    /* DP for rows 2 … n (n‑1 transitions) */
    for (idx_t step = 1; step <= n-1; ++step) {
        memset(next, 0, STATE_CNT * sizeof(seq_t));
        for (size_t s = 0; s < STATE_CNT; ++s) {
            seq_t val = cur[s];
            if (val == 0)
                continue;
            size_t pos = adj_start[s];
            uint16_t len = adj_len[s];
            for (uint16_t k = 0; k < len; ++k) {
                uint8_t nxt = adj_next[pos + k];
                size_t ns = ((s & 0xFF) << 8) | nxt;
                next[ns] += val;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Enforce bottom row = 0: sum over states that allow transition to 0 */
    seq_t ans = 0;
    for (size_t s = 0; s < STATE_CNT; ++s)
        if (allowed_zero[s])
            ans += cur[s];

    /* Free allocated arrays */
    free_seq_array(cur);
    free_seq_array(next);
    return ans;
}
