/*
 * A207267  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 38, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207267 – number of n×6 binary arrays avoiding
 *  • horizontally: 0 0 0 and 0 1 1
 *  • vertically: 0 1 1 and 1 0 1 (triples of consecutive rows)
 *
 *  The implementation uses dynamic programming over the last two rows.
 *  State (i,j) means that the last two rows are rows[i] and rows[j].
 *  Transition to a new row k is allowed iff the triple (i,j,k)
 *  satisfies the vertical constraints.  The number of arrays of length
 *  n is the sum over all states after processing n rows.
 *
 *  Complexity:   O(n·V³) with V = 18 (fixed constant), i.e. O(n)
 *  Memory:       O(V²)  (324 cells) plus the pre‑computed tables.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute all 6‑bit rows that satisfy the horizontal rules.
     * There are 18 such rows (for 6 columns). */
    static uint8_t rows[64];
    static size_t V = 0;
    /* Compatibility table for (row i , row j , row k) being valid. */
    static char compat[64*64*64];
    static bool initialized = false;

    if (!initialized) {
        size_t cnt = 0;
        for (int b = 0; b < 64; ++b) {
            bool ok = true;
            /* Check every sliding window of 3 bits in the 6-bit word. */
            for (int j = 0; j <= 3; ++j) {
                int t = (b >> j) & 7;          /* bits j..j+2 */
                if (t == 0 || t == 3) {        /* 000 or 011 forbidden */
                    ok = false;
                    break;
                }
            }
            if (ok) rows[cnt++] = (uint8_t)b;
        }
        V = cnt;        /* should be 18 */

        /* Build the compatibility table. */
        for (size_t i = 0; i < V; ++i)
            for (size_t j = 0; j < V; ++j)
                for (size_t k = 0; k < V; ++k) {
                    bool good = true;
                    for (int col = 0; col < 6; ++col) {
                        int a = (rows[i] >> col) & 1;
                        int b = (rows[j] >> col) & 1;
                        int c = (rows[k] >> col) & 1;
                        if ((a == 0 && b == 1 && c == 1) ||
                            (a == 1 && b == 0 && c == 1)) {
                            good = false;
                            break;
                        }
                    }
                    compat[(size_t)i*V*V + (size_t)j*V + k] = good ? 1 : 0;
                }
        initialized = true;
    }

    /* Offset 1; values for n < 1 are undefined, return 0. */
    if (n == 0) return (seq_t)0;

    /* Base cases. */
    if (n == 1) return (seq_t)V;            /* one row, any of the V rows */
    if (n == 2) return (seq_t)V * (seq_t)V; /* two rows, no vertical rule */

    /* DP: states are pairs (prevRow, currRow). */
    size_t states = V * V;
    /* Allocate the state arrays.  Use explicit cast from void* to seq_t*. */
    seq_t *dp_prev = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(states, sizeof(seq_t));

    /* Initialisation: all pairs of rows are allowed for the first two rows. */
    for (size_t idx = 0; idx < states; ++idx) dp_prev[idx] = (seq_t)1;

    /* Process rows 3 … n. */
    for (idx_t row = 3; row <= n; ++row) {
        for (size_t idx = 0; idx < states; ++idx) dp_next[idx] = (seq_t)0;

        for (size_t i = 0; i < V; ++i) {
            for (size_t j = 0; j < V; ++j) {
                seq_t val = dp_prev[i*V + j];
                if (val == (seq_t)0) continue;
                for (size_t k = 0; k < V; ++k) {
                    if (compat[(size_t)i*V*V + (size_t)j*V + k]) {
                        dp_next[j*V + k] += val;
                    }
                }
            }
        }

        /* swap buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* Sum all valid end‑states. */
    seq_t result = (seq_t)0;
    for (size_t idx = 0; idx < states; ++idx) result += dp_prev[idx];

    free(dp_prev);
    free(dp_next);
    return result;
}
