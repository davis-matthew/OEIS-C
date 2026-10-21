/*
 * A223985  Number of nX6 0..3 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A223985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 arrays over {0,1,2,3} with rows unimodal
 *            and columns nondecreasing. The sequence begins 610, 52591, …
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 28, u128 n <= 210,
 *            big any n
 * Helpers:   init_unimodal_mask, is_unimodal_code
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/init_unimodal_mask.h"
#include "../utils/is_unimodal_code.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard anyway */

    /* Unimodal mask: 1 means the 6‑tuple is allowed as a row. */
    static bool unimodal[4096];
    init_unimodal_mask(unimodal);

    const unsigned int SIZE = 4096;

    /* dp[code] holds the number of sequences of the current length that
       end with the 6‑tuple encoded by *code*.  The array is reinitialized
       for every call. */
    seq_t dp[SIZE];
    seq_t next_arr[SIZE];

    /* length 1: each unimodal row occurs exactly once. */
    for (unsigned int i = 0; i < SIZE; ++i)
        dp[i] = unimodal[i] ? (seq_t)1 : (seq_t)0;

    /* Iterate from length 2 up to n.  During each step we compute the
       cumulative sums over the partial order “componentwise ≤”. */
    for (idx_t step = 2; step <= n; ++step) {

        /* Start by copying the current dp into next_arr. */
        for (unsigned int i = 0; i < SIZE; ++i)
            next_arr[i] = dp[i];

        /* Prefix‑sum transform over the six dimensions.  After processing
           all six, next_arr[code] contains Σ dp[prev] over all prev ≤ code. */
        for (int dim = 0; dim < 6; ++dim) {
            unsigned int shift = 1u << (2 * dim);   /* 4^dim */

            for (unsigned int code = 0; code < SIZE; ++code) {
                /* If the digit in this dimension is 0, nothing to add. */
                if (((code >> (2 * dim)) & 3U) > 0) {
                    next_arr[code] += next_arr[code - shift];
                }
            }
        }

        /* After the prefix sums, zero out the non‑unimodal states so they
           cannot appear as rows in further steps. */
        for (unsigned int i = 0; i < SIZE; ++i)
            if (!unimodal[i]) next_arr[i] = (seq_t)0;

        /* Prepare for the next step: dp = next_arr. */
        for (unsigned int i = 0; i < SIZE; ++i)
            dp[i] = next_arr[i];
    }

    /* Sum all sequences of length n (ending with any unimodal row). */
    seq_t result = 0;
    for (unsigned int i = 0; i < SIZE; ++i)
        result += dp[i];

    return result;
}
