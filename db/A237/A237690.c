/*
 * A237690  Number of (n+1) X (3+1) 0..3 arrays with the maximum plus the
 *          upper median plus the minimum minus the lower median of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A237690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   subblock_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A237690
    Number of (n+1) × (3+1) 0..3 arrays with the maximum plus the upper median plus the minimum minus the lower median of every 2 × 2 subblock equal.
    The implementation uses a dynamic‑programming approach over the 256 possible rows.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/subblock_value.h"

/* Row digits (base‑4 representation) for each of the 256 rows */
static uint8_t row_digits[256][4];

/* pair_s[i][j] = S (0..6) when rows i,j form a valid transition,
   otherwise -1. */
static int8_t pair_s[256][256];

/* For each S (0..6) and source row i: number of outgoing edges
   with that S. 0 if row i can’t transition with S. */
static uint16_t outdeg[7][256];

/* For each S and source row i: offset into the adjacency array */
static size_t offset[7][257];

/* For each S: contiguous list of destination rows (encoded as uint16_t) */
static uint16_t *edge_list[7] = { NULL };

/* DP tables: cur[s][j] – number of partial arrays ending at row j
   with constant S = s after the current number of transitions. */
static seq_t cur[7][256];
static seq_t nxt[7][256];

/* Memoised results for a(n).  a_cache[0] unused, a_cache[1] = a(1).. */
static seq_t *a_cache      = NULL;
static idx_t  a_cache_len  = 0;      /* highest index stored (>=1) */
static size_t a_cache_size = 0;      /* allocated size */

/* Current number of transitions (edges) processed in cur. */
static idx_t current_n = 0;

/* Initialise all static tables and compute a(1). */
static void init_data(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* 1. Pre‑compute row digits. */
    for (int r = 0; r < 256; ++r) {
        uint8_t x = (uint8_t)r;
        row_digits[r][0] = x & 3;
        row_digits[r][1] = (x >> 2) & 3;
        row_digits[r][2] = (x >> 4) & 3;
        row_digits[r][3] = (x >> 6) & 3;
    }

    /* 2. Determine compatible pairs and their S values. */
    memset(outdeg, 0, sizeof(outdeg));
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            uint8_t s0 = subblock_value(row_digits[i][0], row_digits[i][1],
                                         row_digits[j][0], row_digits[j][1]);
            uint8_t s1 = subblock_value(row_digits[i][1], row_digits[i][2],
                                         row_digits[j][1], row_digits[j][2]);
            uint8_t s2 = subblock_value(row_digits[i][2], row_digits[i][3],
                                         row_digits[j][2], row_digits[j][3]);
            if (s0 == s1 && s1 == s2) {
                pair_s[i][j] = (int8_t)s0;
                ++outdeg[s0][i];
            } else
                pair_s[i][j] = -1;
        }
    }

    /* 3. Build adjacency lists for each S. */
    for (int s = 0; s < 7; ++s) {
        offset[s][0] = 0;
        for (int i = 0; i < 256; ++i)
            offset[s][i + 1] = offset[s][i] + outdeg[s][i];
        size_t total = offset[s][256];
        if (total == 0) {
            edge_list[s] = NULL;
            continue;
        }
        edge_list[s] = (uint16_t *)malloc(total * sizeof(uint16_t));

        size_t pos[256];
        for (int i = 0; i < 256; ++i) pos[i] = offset[s][i];

        for (int i = 0; i < 256; ++i) {
            for (int j = 0; j < 256; ++j) {
                if (pair_s[i][j] == (int8_t)s) {
                    edge_list[s][pos[i]++] = (uint16_t)j;
                }
            }
        }
    }

    /* 4. Initialise DP for n = 1 (two rows). */
    for (int s = 0; s < 7; ++s)
        for (int j = 0; j < 256; ++j)
            cur[s][j] = 0;

    for (int s = 0; s < 7; ++s)
        for (int i = 0; i < 256; ++i)
            for (size_t k = offset[s][i]; k < offset[s][i + 1]; ++k) {
                uint16_t j = edge_list[s][k];
                cur[s][j] += 1;
            }

    /* 5. Compute a(1). */
    seq_t sum1 = 0;
    for (int s = 0; s < 7; ++s)
        for (int j = 0; j < 256; ++j)
            sum1 += cur[s][j];

    /* 6. Allocate cache and store a(1). */
    a_cache_size = 2;  /* indices 0..1 */
    a_cache = (seq_t *)malloc(a_cache_size * sizeof(seq_t));
    a_cache[0] = 0;      /* unused */
    a_cache[1] = sum1;
    a_cache_len = 1;

    current_n = 1;
}

/*------------------------------------------------------------------*/
/*  Function to retrieve the n‑th term of the sequence.             */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* sequence defined for n >= 1 */

    init_data();              /* ensure tables are prepared */

    /* Extend the precomputation up to n. */
    while (a_cache_len < n) {
        /* zero the next table */
        for (int s = 0; s < 7; ++s)
            memset(nxt[s], 0, 256 * sizeof(seq_t));

        /* propagate counts: from each row i to each neighbor j */
        for (int s = 0; s < 7; ++s) {
            for (int i = 0; i < 256; ++i) {
                seq_t val = cur[s][i];
                if (val == 0) continue;
                for (size_t k = offset[s][i]; k < offset[s][i + 1]; ++k) {
                    uint16_t j = edge_list[s][k];
                    nxt[s][j] += val;
                }
            }
        }

        /* copy back to cur */
        for (int s = 0; s < 7; ++s)
            memcpy(cur[s], nxt[s], 256 * sizeof(seq_t));

        ++current_n;   /* one more transition performed */

        /* compute a(current_n) */
        seq_t sum = 0;
        for (int s = 0; s < 7; ++s)
            for (int j = 0; j < 256; ++j)
                sum += cur[s][j];

        /* enlarge cache if necessary */
        if (a_cache_len + 1 >= a_cache_size) {
            size_t new_size = a_cache_size * 2;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
            a_cache = tmp;
            a_cache_size = new_size;
        }

        a_cache[a_cache_len + 1] = sum;
        ++a_cache_len;
    }

    return a_cache[n];
}
