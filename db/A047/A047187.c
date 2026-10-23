/*
 * A047187  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(⌊(n-2)/2⌋, ⌊(n-⌊(n-2)/2⌋)/4⌋)}
 *            binomial(⌊(n-2)/2⌋, k) * binomial(n-⌊(n-2)/2⌋, 4k)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                        /* for size_t */
#include "../utils/binomial.h"              /* provides seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Return 0 for n < 2 to avoid unsigned underflow in L calculation.   */
    if (n <= 1) return 0;

    /* L = ⌊(n-2)/2⌋  (lower part),  U = n - L  (upper part).            */
    idx_t L = (n - 2) / 2;
    idx_t U = n - L;

    /* Upper bound for k:  k ≤ L  and 4k ≤ U  →  k ≤ U/4.               */
    idx_t maxK = U / 4;
    if (maxK > L) maxK = L;
    if (maxK == 0) return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= maxK; ++k) {
        seq_t low  = binomial(L, k);          /* choose k from lower part   */
        seq_t high = binomial(U, 4 * k);       /* choose 4k from upper part  */
        sum += low * high;                    /* accumulate product          */
    }
    return sum;
}
