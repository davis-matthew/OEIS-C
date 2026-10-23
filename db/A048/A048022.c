/*
 * A048022  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} C(⌊(n‑2)/3⌋, 2k) · C(n‑⌊(n‑2)/3⌋, 3k)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(x,y) denotes the binomial coefficient */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the split point t = ⌊(n-2)/3⌋.  For n < 3 the value is 0. */
    idx_t t = (n >= 3) ? (n - 2) / 3 : 0;
    /* The remaining elements are considered “large”. */
    idx_t large = n - t;

    seq_t result = 0;

    /* The maximum possible k is limited by the smaller of t/2 and large/3. */
    idx_t maxk = t / 2;
    if ((large / 3) < maxk)
        maxk = large / 3;

    /* Sum over all admissible k. */
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t small_needed = 2 * k;
        idx_t large_needed = 3 * k;

        /* By construction small_needed ≤ t and large_needed ≤ large
           but we keep the checks for safety if the logic is altered. */
        if (small_needed > t || large_needed > large)
            continue;

        seq_t small = binomial(t, small_needed);
        seq_t larg  = binomial(large, large_needed);
        result += small * larg;
    }

    return result;
}
