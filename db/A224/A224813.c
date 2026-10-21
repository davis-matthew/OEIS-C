/*
 * A224813  Number of subsets of {1,2,...,n-12} without differences equal to
 *          2, 4, 6, 8, 10 or 12.
 * https://oeis.org/A224813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #subsets of {1,2,…,n‑12} avoiding differences
 *            2,4,6,8,10,12. All initial values a(0) … a(12) equal 1
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 104, u64 n <= 202,
 *            u128 n <= 397, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a DP over the last 12 positions. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* For n ≤ 12 the set {1,…,n‑12} is empty – only the empty subset */
    if (n <= 12)
        return (seq_t)1;

    /* Number of positions to consider */
    const idx_t N = n - 12;

    /* There are 2¹² possible masks of the last 12 chosen vertices */
    const size_t STATES   = (size_t)1 << 12;
    const size_t MASK_ALL = STATES - 1;

    /* Bits corresponding to forbidden distances: 2,4,6,8,10,12  */
    const size_t FORBID =
          ((size_t)1 << 1)
        | ((size_t)1 << 3)
        | ((size_t)1 << 5)
        | ((size_t)1 << 7)
        | ((size_t)1 << 9)
        | ((size_t)1 << 11);

    /* DP arrays for current and next position */
    seq_t *dp   = alloc_seq_array(STATES);
    seq_t *next = alloc_seq_array(STATES);
    if (!dp || !next)  /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0;

    /* Initialise: before the first position no vertices have been chosen */
    for (size_t i = 0; i < STATES; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Process each position sequentially */
    for (idx_t pos = 1; pos <= N; ++pos) {
        /* reset next array */
        for (size_t i = 0; i < STATES; ++i)
            next[i] = 0;

        for (size_t mask = 0; mask < STATES; ++mask) {
            seq_t val = dp[mask];
            if (val == 0)
                continue;

            /* Option 1: do not select current position */
            size_t mask_no = (mask << 1) & MASK_ALL;
            next[mask_no] += val;

            /* Option 2: select current position, if allowed */
            if ((mask & FORBID) == 0) {
                size_t mask_yes = ((mask << 1) | 1) & MASK_ALL;
                next[mask_yes] += val;
            }
        }
        /* swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* Sum all counts – any final state is valid */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i)
        total += dp[i];

    free_seq_array(dp);
    free_seq_array(next);
    return total;
}
