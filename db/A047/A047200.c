/*
 * A047200  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= n/3.
 * https://oeis.org/A047200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min( floor(n/3)/3 , floor((n - floor(n/3))/2) )}
 *            binom( floor(n/3) , 3k ) * binom( n - floor(n/3) , 2k )
 *            (nonempty subsets of {1..n} with exactly 3/5 of elements <= n/3)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by contract; handle trivial case safely */
    if (n < 1)
        return 0;

    idx_t m      = n / 3;          /* floor(n/3) */
    idx_t rest   = n - m;
    idx_t kmax1  = m / 3;          /* floor(m/3) */
    idx_t kmax2  = rest / 2;       /* floor((n-m)/2) */
    idx_t kmax   = kmax1 < kmax2 ? kmax1 : kmax2;

    seq_t total = 0;

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t a_choose = 3 * k;
        idx_t b_choose = 2 * k;

        seq_t term = binomial(m, a_choose);
        term = term * binomial(rest, b_choose);
        total += term;
    }

    return total;
}
