/*
 * A129526  Number of n-bead two-color bracelets with 00 prohibited.
 * https://oeis.org/A129526
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ([Fib(floor(n/2)+2) + (1/n) * Σ_{d|n} φ(n/d) *
 *            (Fib(d-1)+Fib(d+1))] / 2)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   fib, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 2; for smaller n return 0 */
    if (n < 2)
        return 0;

    /* Fib(floor(n/2)+2) */
    idx_t k = n / 2 + 2;            /* floor division */
    seq_t fib_half = fib(k);

    /* sum over all divisors of n */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;          /* first divisor */
            idx_t d2 = n / i;      /* paired divisor */

            /* term for d1 */
            seq_t phi1  = phi(n / d1);
            seq_t fib_d1m1 = fib(d1 - 1);
            seq_t fib_d1p1 = fib(d1 + 1);
            sum += phi1 * (fib_d1m1 + fib_d1p1);

            if (d2 != d1)          /* avoid double counting of square root */
            {
                seq_t phi2  = phi(n / d2);
                seq_t fib_d2m1 = fib(d2 - 1);
                seq_t fib_d2p1 = fib(d2 + 1);
                sum += phi2 * (fib_d2m1 + fib_d2p1);
            }
        }
    }

    /* (1/n) * sum */
    seq_t sum_div_n = sum / n;

    /* average with Fib(floor(n/2)+2) and divide by 2 */
    return (fib_half + sum_div_n) / 2;
}
