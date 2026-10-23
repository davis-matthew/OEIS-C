/*
 * A048000  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= n/3.
 * https://oeis.org/A048000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/5)} binomial(floor(n/3), 4*k) *
 *            binomial(ceil(2*n/3), k)
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 62, u64 n <= 119,
 *            u128 n <= 230, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t nf3   = n / 3;                     /* floor(n/3) */
    idx_t n2c3  = (2 * n + 2) / 3;           /* ceil(2*n/3) */
    idx_t maxk  = n / 5;                     /* floor(n/5)   */
    seq_t sum = 0;

    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t r = 4 * k;
        if (r > nf3) continue;                /* binomial would be zero */
        sum += binomial(nf3, r) * binomial(n2c3, k);
    }
    return sum;
}
