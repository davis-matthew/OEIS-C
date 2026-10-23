/*
 * A126542  Number of base 21 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑21 n‑digit strings (digits 0–20) where *
 *            each pair of adjacent digits differs by at most 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types: seq_t is an integer type large enough to hold the
 * result (built‑in unsigned integer or arbitrary‑precision class).
 * idx_t is an unsigned integer type (uint64_t). */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0ULL) {
        return (seq_t)1U;
    }

    const idx_t DIGITS    = 21U;    /* digits 0 … 20 */
    const idx_t MAX_DIFF  = 5U;     /* allowed absolute difference */

    /* Allocate two arrays: one for the current, one for the next DP layer. */
    seq_t *dp   = (seq_t *)malloc((size_t)DIGITS * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((size_t)DIGITS * sizeof(seq_t));
    if (!dp || !next) {
        /* Allocation failed; return 0 as a safe fallback. */
        free(dp);
        free(next);
        return (seq_t)0U;
    }

    /* Initialise dp for strings of length 1: each digit occurs exactly once. */
    for (idx_t d = 0ULL; d < DIGITS; ++d) {
        dp[d] = (seq_t)1U;
    }

    /* Build up to length n. */
    for (idx_t len = 2ULL; len <= n; ++len) {

        /* Reset next array to zero. */
        for (idx_t e = 0ULL; e < DIGITS; ++e) {
            next[e] = (seq_t)0U;
        }

        /* Transition from dp[d] to all allowed next digits e. */
        for (idx_t d = 0ULL; d < DIGITS; ++d) {
            seq_t val = dp[d];
            if (val == (seq_t)0U) {
                continue;
            }

            idx_t lo = (d >= MAX_DIFF) ? d - MAX_DIFF : 0ULL;
            idx_t hi = (d + MAX_DIFF < DIGITS) ? d + MAX_DIFF : DIGITS - 1ULL;

            for (idx_t e = lo; e <= hi; ++e) {
                next[e] = next[e] + val;
            }
        }

        /* Swap dp and next for the next iteration. */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* Sum over all ending digits to get the total number of strings. */
    seq_t result = (seq_t)0U;
    for (idx_t d = 0ULL; d < DIGITS; ++d) {
        result = result + dp[d];
    }

    /* Free the allocated arrays. */
    free(dp);
    free(next);

    return result;
}
