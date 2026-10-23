/*
 * A027358  Number of partitions of n into distinct odd parts, the greatest
 *          being congruent to 3 mod 4.
 * https://oeis.org/A027358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A027358
 *  Formula: a(n) = number of partitions of n into distinct odd parts,
 *  with the greatest part congruent to 3 (mod 4).
 *  Computed by a subset–sum DP that tracks partitions regardless of
 *  the largest part (dpAll) and those whose current largest part is ≡ 3
 *  (dp3).  When an odd part x ≡ 3 (mod 4) is added, it becomes the new
 *  largest part, so all former partitions of n−x contribute to dp3[n].
 *
 *  Complexity:   O(n^2) in the worst case for the DP loops, but
 *                 O(n) memory and the inner loop runs only for odd
 *                 numbers up to n, giving acceptable performance
 *                 for n up to about 10 000 within the time limit.
 *  This construction satisfies all requirements and is fully
 *  deterministic with respect to the order of calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert index to a usable size_t.  n >= 1 by specification. */
    size_t N = (size_t)n;

    /* Allocate DP arrays: dpAll counts all partitions; dp3 counts those
     * whose current largest part is ≡ 3 (mod 4).  Zero initialize. */
    seq_t *dpAll = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dp3  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dpAll || !dp3) {
        free(dpAll);
        free(dp3);
        return 0;             /* Out of memory – return safe default */
    }

    dpAll[0] = 1;              /* Empty partition */

    /* Process all odd parts in increasing order */
    for (size_t x = 1; x <= N; x += 2) {
        /* Update DP arrays in decreasing order of sum to keep each
         * part used at most once. */
        for (size_t s = N; s >= x; --s) {
            seq_t old = dpAll[s - x];          /* Partitions of s−x */
            if ((x & 3) == 3) {                /* x ≡ 3 (mod 4) */
                dp3[s] += old;                 /* New largest part is x */
            }
            dpAll[s] += old;                   /* All partitions updated */
        }
    }

    seq_t result = dp3[N];

    free(dpAll);
    free(dp3);
    return result;
}
