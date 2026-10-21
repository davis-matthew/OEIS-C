/*
 * A211279  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 2]
 *          as of [1, 3].
 * https://oeis.org/A211279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A211279
 *  Count n‑letter words over {1,2,3} with equal numbers of
 *  substrings 12 and 13.  The algorithm uses dynamic programming
 *  over the difference d = (#12) − (#13).
 *
 *  For a word of length ℓ ending with 1, appending
 *     1 keeps d unchanged,
 *     2 increases d by 1,
 *     3 decreases d by 1.
 *  A word ending with 2 or 3 is unaffected by the next letter,
 *  thus appending 1,2,or3 keeps d unchanged.
 *
 *  The DP state is represented by three arrays dp1, dp2, dp3
 *  indexed by d offset by ℓ.  At step ℓ the range of possible d is
 *  |d| ≤ ℓ−1, so the arrays need to be sized 2n+1.
 *
 *  Complexity:  O(n²) time, O(n) space.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* Forward declaration of seq_t and idx_t provided by the build. */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Empty word */
    if (n == (idx_t)0) return (seq_t)1;

    /* Size of DP tables: 2n+1 entries for differences from -n to n. */
    const size_t offset = (size_t)n;          /* difference 0  → index offset */
    const size_t sz     = 2 * offset + 1;     /* total number of indices  */

    /* Allocate zero‑initialised arrays. */
    seq_t *dp1 = alloc_seq_array(sz);
    seq_t *dp2 = alloc_seq_array(sz);
    seq_t *dp3 = alloc_seq_array(sz);
    seq_t *ndp1 = alloc_seq_array(sz);
    seq_t *ndp2 = alloc_seq_array(sz);
    seq_t *ndp3 = alloc_seq_array(sz);

    /* Initialise for length 1.  All three letters give empty diff. */
    dp1[offset] = (seq_t)1;
    dp2[offset] = (seq_t)1;
    dp3[offset] = (seq_t)1;

    /* Current reachable range of difference indices.      */
    size_t minIdx = offset;            /* for ℓ = 1 */
    size_t maxIdx = offset;            /* for ℓ = 1 */

    for (idx_t l = 2; l <= n; ++l) {
        /* Extend the words by one letter.  New reachable diffs
         * become one step farther in each direction.          */
        size_t newMin = (minIdx > 0) ? minIdx - 1 : 0;
        size_t newMax = maxIdx + 1;
        if (newMax >= sz) newMax = sz - 1;   /* guard, should not happen */

        /* Clear new DP tables for indices that will be written. */
        for (size_t i = newMin; i <= newMax; ++i) {
            ndp1[i] = ndp2[i] = ndp3[i] = (seq_t)0;
        }

        /* Transition from all current states. */
        for (size_t i = minIdx; i <= maxIdx; ++i) {
            seq_t v1 = dp1[i];
            seq_t v2 = dp2[i];
            seq_t v3 = dp3[i];

            if (v1) {
                ndp1[i]     += v1;           /* 1→1 */
                ndp2[i + 1] += v1;           /* 1→2 */
                ndp3[i - 1] += v1;           /* 1→3 */
            }
            if (v2) {
                ndp1[i] += v2;               /* 2→1 */
                ndp2[i] += v2;               /* 2→2 */
                ndp3[i] += v2;               /* 2→3 */
            }
            if (v3) {
                ndp1[i] += v3;               /* 3→1 */
                ndp2[i] += v3;               /* 3→2 */
                ndp3[i] += v3;               /* 3→3 */
            }
        }

        /* Swap old and new tables for next iteration. */
        seq_t *t1 = dp1; dp1 = ndp1; ndp1 = t1;
        seq_t *t2 = dp2; dp2 = ndp2; ndp2 = t2;
        seq_t *t3 = dp3; dp3 = ndp3; ndp3 = t3;

        /* Update reachable index range. */
        minIdx = newMin;
        maxIdx = newMax;
    }

    /* Sum all words of length n with difference zero. */
    seq_t result = dp1[offset] + dp2[offset] + dp3[offset];

    /* No need to free in case of arbitrary‑precision build. */
    free(dp1);
    free(dp2);
    free(dp3);
    free(ndp1);
    free(ndp2);
    free(ndp3);

    return result;
}
