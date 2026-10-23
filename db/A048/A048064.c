/*
 * A048064  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, 3k≤floor((n-4)/2) , k≤n-floor((n-4)/2)}
 *            binomial(floor((n-4)/2), 3k) * binomial(n-floor((n-4)/2), k)
 *            (Sum over k where subsets have exactly 3/4 of their elements ≤
 *            (n-4)/2)
 * Range:     u8 n <= 16, u16 n <= 25, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 4, the expression (n-4) would underflow an unsigned idx_t.
       All terms are 0 for n = 1,2,3,4,5,6,7,8,9, so we can return early. */
    if (n < 4)
        return 0;

    /* Lower half size */
    idx_t L = (n - 4) / 2;          /* floor((n-4)/2) */
    idx_t U = n - L;                /* elements > (n-4)/2 */

    /* k must satisfy 3k ≤ L and k ≤ U */
    idx_t kmax = L / 3;
    if (kmax > U)
        kmax = U;

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t low  = binomial(L, 3 * k);
        /* If the binomial is zero, parameters were invalid; skip. */
        if (!low)
            continue;
        seq_t high = binomial(U, k);
        if (!high)
            continue;
        sum += low * high;
    }
    return sum;
}
