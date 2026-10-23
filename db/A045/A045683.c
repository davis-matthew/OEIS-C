/*
 * A045683  Number of 2n-bead balanced binary necklaces of fundamental period
 *          2n which are equivalent to their reverse, complement and reversed
 *          complement.
 * https://oeis.org/A045683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0, otherwise a(n) = Σ_{d|n, d odd} μ(d) *
 *            2^⌊(n/d – 1)/2⌋ (OEIS A045683)
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 66, u64 n <= 130,
 *            u128 n <= 200, big any n
 * Helpers:   mu_of, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* Enumerate all positive divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* First divisor d */
        if ((d & 1ULL) != 0) {     /* only odd divisors */
            int mu = mu_of(d);
            if (mu != 0) {
                idx_t exp = (n / d - 1) / 2;   /* floor((n/d-1)/2) */
                seq_t val = pow2(exp);
                if (mu > 0)
                    sum_pos += val;
                else
                    sum_neg += val;
            }
        }

        /* Second divisor n/d if different from d */
        idx_t d2 = n / d;
        if (d2 == d)               /* already processed */
            continue;

        if ((d2 & 1ULL) != 0) {
            int mu = mu_of(d2);
            if (mu != 0) {
                idx_t exp = (n / d2 - 1) / 2;
                seq_t val = pow2(exp);
                if (mu > 0)
                    sum_pos += val;
                else
                    sum_neg += val;
            }
        }
    }

    return sum_pos - sum_neg;
}
