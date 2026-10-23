/*
 * A047170  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= n/2.
 * https://oeis.org/A047170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} binomial(floor(n/2), 5*k)*binomial(ceil(n/2),
 *            k)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 49, u64 n <= 91, u128 n <= 177,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Return the number of nonempty subsets of {1,2,…,n} in which exactly
   5/6 of the elements are <= n/2.  */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t L = n / 2;
    /* ceil(n/2) = n - floor(n/2) */
    idx_t R = n - L;

    seq_t total = 0;
    idx_t kmax = L / 5;          /* k must satisfy 5*k <= L */

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t c1 = binomial(L, 5 * k);
        seq_t c2 = binomial(R, k);
        total += c1 * c2;        /* addition and multiplication are exact */
    }
    return total;
}
