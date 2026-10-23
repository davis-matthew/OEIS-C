/*
 * A048068  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k≥1} C(L,4k)*C(U,k) where L = floor((n-4)/2), U = n
 *            - L Number of nonempty subsets of {1,…,n} in which exactly 4/5
 *            of the elements are ≤ (n-4)/2
 * Range:     u8 n <= 17, u16 n <= 27, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 4)          /* for n ≤ 4 there can be no such subsets           */
        return 0;

    /* Size of the lower part (≤ (n-4)/2)                              */
    idx_t L = (n - 4) / 2;
    /* Size of the upper part (> (n-4)/2)                              */
    idx_t U = n - L;

    /* Max k: 4k ≤ L  and  k ≤ U                                       */
    idx_t kmax = L / 4;          /* because 4k ≤ L  →  k ≤ L/4                */
    if (kmax > U)
        kmax = U;

    seq_t result = 0;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        seq_t binL = binomial(L, 4 * k);
        seq_t binU = binomial(U, k);
        result += binL * binU;
    }

    return result;
}
