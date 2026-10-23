/*
 * A106520  a(n) = A068875(n-1) - A003239(n).
 * https://oeis.org/A106520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/n) * binomial(2*n-2, n-1) - (1/(2*n)) * Sum_{d|n}
 *            phi(d) * binomial(2*n/d, n/d)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/phi.h"

/* Computes A106520.  The function has external linkage.  */
seq_t a(idx_t n)
{
    /* n is expected to be ≥ 1; behaviour for n < 1 is undefined.  */

    /* First term: (2/n) * binomial(2*n-2, n-1)  */
    seq_t first = binom(2 * n - 2, n - 1);   /* binomial(2n-2, n-1)   */
    first *= 2;                              /* multiply by 2          */
    first /= n;                              /* exact division by n   */

    /* Second term: (1/(2*n)) * Σ_{d|n} phi(d) * binomial(2*n/d, n/d)  */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k = n / d;

            /* divisor d */
            seq_t phi_d = phi(d);
            seq_t bin_d = binom(2 * k, k);
            sum += phi_d * bin_d;

            /* complementary divisor k = n/d, if distinct */
            if (d != k) {
                seq_t phi_k = phi(k);
                seq_t bin_k = binom(2 * d, d);
                sum += phi_k * bin_k;
            }
        }
    }

    seq_t second = sum;
    second /= (seq_t)(2 * n);                /* divide by 2*n, exact division */

    /* Result is the difference of the two terms */
    return first - second;
}
