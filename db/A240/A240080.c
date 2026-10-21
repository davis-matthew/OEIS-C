/*
 * A240080  Number of partitions of n such that m(greatest part) >= m(1),
 *          where m = multiplicity.
 * https://oeis.org/A240080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n whose largest part occurs at
 *            least as many times as the part 1
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 1;      /* empty partition */

    /* Allocate arrays for partitions with largest part <= L-1
       and their prefix sums.  */
    seq_t *cur = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *pref = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!cur || !pref) {            /* allocation failure */
        free(cur); free(pref);
        return 0;
    }

    /* Initial state: only the empty partition is possible. */
    cur[0] = 1;

    /* Result starts with the unique partition consisting entirely of 1's. */
    seq_t result = 1;

    for (idx_t L = 2; L <= n; ++L) {
        /* Build prefix sums of the current cur array. */
        pref[0] = cur[0];
        for (idx_t i = 1; i <= n; ++i)
            pref[i] = pref[i - 1] + cur[i];

        /* Count partitions whose largest part is L. */
        idx_t maxU = n / L;
        for (idx_t u = 1; u <= maxU; ++u) {
            idx_t remaining = n - u * L;            /* n - u*L  */
            idx_t tmax = remaining < u ? remaining : u;   /* max number of 1's */

            /* Sum cur[remaining - t] for t = 0 .. tmax */
            idx_t left = remaining - tmax;          /* smallest index in the sum */
            seq_t partSum;
            if (left > 0)
                partSum = pref[remaining] - pref[left - 1];
            else
                partSum = pref[remaining];

            result += partSum;
        }

        /* Update cur to include part L so that it becomes available in
           the next iteration (parts <= L). */
        for (idx_t i = L; i <= n; ++i)
            cur[i] += cur[i - L];
    }

    free(cur);
    free(pref);
    return result;
}
