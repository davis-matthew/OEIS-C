/*
 * A207344  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×6 binary arrays avoiding horizontal 000,
 *            horizontal 011, vertical 000 and vertical 111. This is computed
 *            by a transfer‑matrix dynamic programming over the allowed rows
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/free_seq_arr.h"

/* Helper to allocate an array of seq_t with value‑initialisation */
static seq_t* alloc_seq(size_t n) {
#ifdef __cplusplus
    return new seq_t[n]();   /* value‑initialise each element to 0 */
#else
    return (seq_t*)calloc(n, sizeof(seq_t));   /* zero initialisation */
#endif
}

/* Check whether a 6‑bit row satisfies the horizontal constraints
   (no 000 and no 011).  Bits are numbered 0..5 from LSB to MSB. */
static int valid_row(uint8_t r) {
    int i;
    for (i = 0; i <= 3; ++i) {
        int b0 = (r >> i)     & 1;
        int b1 = (r >> (i+1)) & 1;
        int b2 = (r >> (i+2)) & 1;
        if (b0 == 0 && b1 == 0 && b2 == 0) return 0;
        if (b0 == 0 && b1 == 1 && b2 == 1) return 0;
    }
    return 1;
}

/* Main sequence function required by the build system */
seq_t a(idx_t n)
{
    /* Step 1: enumerate all valid 6‑bit rows */
    uint8_t rows[64];
    int ridx[64];
    int V = 0;            /* number of valid rows */
    int i, j;
    for (i = 0; i < 64; ++i) {
        if (valid_row((uint8_t)i)) {
            rows[V] = (uint8_t)i;
            ridx[i] = V;
            ++V;
        }
    }

    /* Special cases n = 1 and n = 2 */
    if (n == 1) {
        return (seq_t)V;
    }
    if (n == 2) {
        return (seq_t)V * (seq_t)V;
    }

    /* Step 2: precompute transition lists for each pair of rows */
    size_t S = (size_t)V * (size_t)V;          /* number of (row_i, row_{i+1}) states */
    uint8_t *next_count = (uint8_t*)calloc(S, sizeof(uint8_t));
    uint8_t **next_list  = (uint8_t**)calloc(S, sizeof(uint8_t*));
    for (i = 0; i < (int)S; ++i) {
        next_list[i] = (uint8_t*)calloc(V, sizeof(uint8_t));
    }

    for (i = 0; i < V; ++i) {
        for (j = 0; j < V; ++j) {
            uint8_t a = rows[i];
            uint8_t b = rows[j];
            size_t pair = (size_t)i * V + j;
            uint8_t *succ = next_list[pair];
            uint8_t counter = 0;

            /* Try each possible third row c */
            int c;
            for (c = 0; c < V; ++c) {
                uint8_t first  = a;
                uint8_t second = b;
                uint8_t third  = rows[c];

                int ok = 1;
                int col;
                for (col = 0; col < 6; ++col) {
                    int s = ((first  >> col) & 1) +
                            ((second >> col) & 1) +
                            ((third  >> col) & 1);
                    if (s == 0 || s == 3) { ok = 0; break; }
                }
                if (ok) {
                    succ[counter++] = (uint8_t)c;
                }
            }
            next_count[pair] = counter;
        }
    }

    /* Step 3: dynamic programming over rows */
    seq_t *dp_prev = alloc_seq(S);
    seq_t *dp_cur  = alloc_seq(S);

    /* For n = 2, all pairs are allowed */
    for (size_t idx = 0; idx < S; ++idx) dp_prev[idx] = (seq_t)1;

    for (idx_t row = 3; row <= n; ++row) {
        /* zero current array */
        for (size_t idx = 0; idx < S; ++idx) dp_cur[idx] = (seq_t)0;

        /* iterate over all states (a,b) */
        for (i = 0; i < V; ++i) {
            for (j = 0; j < V; ++j) {
                size_t pair = (size_t)i * V + j;
                seq_t ways = dp_prev[pair];
                if (ways == (seq_t)0) continue;

                uint8_t *succ = next_list[pair];
                uint8_t counter = next_count[pair];
                uint8_t k;
                for (k = 0; k < counter; ++k) {
                    size_t new_pair = (size_t)j * V + succ[k];
                    dp_cur[new_pair] += ways;
                }
            }
        }
        /* swap dp_prev and dp_cur */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    /* Sum over all final states */
    seq_t ans = (seq_t)0;
    for (size_t idx = 0; idx < S; ++idx) ans += dp_prev[idx];

    /* Clean up */
    for (i = 0; i < (int)S; ++i) free(next_list[i]);
    free(next_list);
    free(next_count);
    free_seq_arr(dp_prev);
    free_seq_arr(dp_cur);

    return ans;
}
