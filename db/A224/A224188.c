/*
 * A224188  Number of n X 6 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 arrays of 0..2 whose rows are unimodal and
 *            columns are nondecreasing. Computed via dynamic programming on
 *            the 148 unimodal 6‑tuples (vectors over {0,1,2})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 85, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* ----- static data common to all calls ----- */
    static bool initialized = false;
    static bool unim[729];
    static uint8_t coord[729][6];
    static int pow3[6];

    if (!initialized) {
        /* powers of 3 */
        int p = 1;
        for (int d = 0; d < 6; ++d) {
            pow3[d] = p;
            p *= 3;
        }

        /* coordinate and unimodality tables */
        for (int idx = 0; idx < 729; ++idx) {
            int t = idx;
            for (int d = 0; d < 6; ++d) {
                coord[idx][d] = t % 3;
                t /= 3;
            }

            /* arr[0]..arr[5] correspond to columns 0..5 (most significant first) */
            int arr[6];
            for (int j = 0; j < 6; ++j)
                arr[j] = coord[idx][5 - j];

            bool is_unim = false;
            for (int peak = 0; peak < 6 && !is_unim; ++peak) {
                bool ok = true;
                for (int i = 0; i < peak; ++i)
                    if (arr[i] > arr[i + 1]) { ok = false; break; }
                for (int i = peak; i < 5; ++i)
                    if (arr[i] < arr[i + 1]) { ok = false; break; }
                if (ok) is_unim = true;
            }
            unim[idx] = is_unim;
        }
        initialized = true;
    }

    /* ----- dynamic programming arrays ----- */
    const size_t SIZE = 729;
    seq_t *dp      = (seq_t *)calloc(SIZE, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(SIZE, sizeof(seq_t));
    seq_t *cum     = (seq_t *)calloc(SIZE, sizeof(seq_t));
    if (!dp || !dp_next || !cum) {
        /* allocation failure – return 0 to avoid crash */
        free(dp); free(dp_next); free(cum);
        return 0;
    }

    /* initial state: length 1, each unimodal pattern can appear */
    for (size_t i = 0; i < SIZE; ++i)
        dp[i] = unim[i] ? 1 : 0;

    if (n == 1) {
        /* total is simply the number of unimodal patterns (148) */
        seq_t total = 0;
        for (size_t i = 0; i < SIZE; ++i)
            if (unim[i]) total += dp[i];
        free(dp); free(dp_next); free(cum);
        return total;
    }

    /* iterate over remaining rows */
    for (idx_t step = 2; step <= n; ++step) {
        /* copy current dp to cumulative array */
        for (size_t i = 0; i < SIZE; ++i)
            cum[i] = dp[i];

        /* compute 6‑dimensional prefix sums over all tuples */
        for (int d = 0; d < 6; ++d) {
            int shift = pow3[d];
            for (size_t i = 0; i < SIZE; ++i) {
                if (coord[i][d]) {
                    size_t j = i - shift;
                    cum[i] += cum[j];
                }
            }
        }

        /* new dp array: chains ending at pattern i */
        for (size_t i = 0; i < SIZE; ++i)
            dp_next[i] = unim[i] ? cum[i] : 0;

        /* swap dp and dp_next for next iteration */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* final count: sum over all chains of length n ending at a unimodal pattern */
    seq_t total = 0;
    for (size_t i = 0; i < SIZE; ++i)
        if (unim[i]) total += dp[i];

    free(dp); free(dp_next); free(cum);
    return total;
}
