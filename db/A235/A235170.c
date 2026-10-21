/*
 * A235170  Number of (n+1) X (2+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+1) × 3 arrays over {0,…,7} such that
 *   – no two horizontally or vertically adjacent cells are equal,
 *   – each 2×2 subblock has diagonal sum differing from its
 *     antidiagonal sum by ±4 (i.e. |(A,B;C,D) diag - antidiag| = 4).
 * The function uses a transfer‑matrix dynamic programming approach.
 * Complexity: O((n+1) · E) where E ≈ 2000 is the number of admissible
 * transitions between rows; this is well below the time limit for n ≤ 10000.
 */

#include <stdint.h>
#include <stdlib.h>

/* The required function ----------------------------------------------------- */
seq_t a(idx_t n);

/* -------------------------- State definition ------------------------------ */
#define MAX_STATES 392                /* maximal number of admissible rows */

static uint8_t  row_digits[MAX_STATES][3];     /* digits of each admissible row */
static uint16_t *row_next[MAX_STATES];         /* adjacency lists */
static uint16_t  row_next_count[MAX_STATES];   /* lengths of the lists */
static uint16_t  state_count = 0;              /* actual number of states   */
static int       precomputed = 0;

/* ----------------------- Build adjacency graph --------------------------- */
static void build_states(void)
{
    int cnt = 0;
    /* enumerate all 3‑tuples with no equal neighbours */
    for (int a = 0; a <= 7; ++a) {
        for (int b = 0; b <= 7; ++b) {
            if (b == a) continue;
            for (int c = 0; c <= 7; ++c) {
                if (c == b) continue;
                row_digits[cnt][0] = (uint8_t)a;
                row_digits[cnt][1] = (uint8_t)b;
                row_digits[cnt][2] = (uint8_t)c;
                ++cnt;
            }
        }
    }
    state_count = (uint16_t)cnt;

    /* build adjacency lists */
    for (int i = 0; i < cnt; ++i) {
        size_t cap = 8;
        uint16_t *list = (uint16_t*)malloc(cap * sizeof(uint16_t));
        size_t used = 0;
        const uint8_t *A = row_digits[i];
        for (int j = 0; j < cnt; ++j) {
            const uint8_t *B = row_digits[j];
            /* vertical adjacency */
            if ((A[0] == B[0]) || (A[1] == B[1]) || (A[2] == B[2]))
                continue;
            /* 2×2 subblock condition */
            int ok = 1;
            for (int k = 0; k < 2; ++k) {
                int d1 = (int)A[k]   + (int)B[k+1];   /* main diag   */
                int d2 = (int)A[k+1] + (int)B[k];     /* anti diag   */
                int diff = d1 > d2 ? d1 - d2 : d2 - d1;
                if (diff != 4) { ok = 0; break; }
            }
            if (!ok) continue;
            /* store transition */
            if (used == cap) {
                cap <<= 1;
                list = (uint16_t*)realloc(list, cap * sizeof(uint16_t));
            }
            list[used++] = (uint16_t)j;
        }
        row_next[i]       = list;
        row_next_count[i] = (uint16_t)used;
    }
}

/* ---------------------------- Sequence function -------------------------- */
seq_t a(idx_t n)
{
    /* initialise once */
    if (!precomputed) {
        build_states();
        precomputed = 1;
    }

    /* offset 1: a(1) is first term.  n==0 is ignored. */
    if (n == 0) return 0;

    idx_t rows = n + 1;                       /* number of rows of the grid */

    /* dynamic programming vectors */
    seq_t *dp_cur  = (seq_t*)calloc(state_count, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(state_count, sizeof(seq_t));
    if (!dp_cur || !dp_next) return 0;       /* allocation failure */

    /* all first rows are allowed */
    for (size_t s = 0; s < state_count; ++s) dp_cur[s] = (seq_t)1;

    /* propagate through the remaining rows */
    for (idx_t r = 1; r < rows; ++r) {
        /* clear next vector */
        for (size_t s = 0; s < state_count; ++s) dp_next[s] = (seq_t)0;

        for (size_t s = 0; s < state_count; ++s) {
            seq_t cur_val = dp_cur[s];
            if (cur_val == (seq_t)0) continue;
            uint16_t *succ   = row_next[s];
            uint16_t  succ_c = row_next_count[s];
            for (uint16_t j = 0; j < succ_c; ++j) {
                dp_next[succ[j]] += cur_val;
            }
        }
        /* swap vectors */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum over final states */
    seq_t total = (seq_t)0;
    for (size_t s = 0; s < state_count; ++s) total += dp_cur[s];

    /* free temporary storage */
    free(dp_cur);
    free(dp_next);

    return total;
}
