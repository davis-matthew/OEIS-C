/*
 * A054629  Number of n-bead necklaces with 10 colors.
 * https://oeis.org/A054629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 0, else a(n) = (1/n) * Σ_{d|n} φ(d) * 10^(n/d)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   phi, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the available φ and power utilities
   and enumerates divisors in O(√n) time, suitable for n up to ~10 000. */
#include "../utils/phi.h"
#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t term1 = phi(d1) * pow_seq_t((seq_t)10, n / d1);
            sum += term1;

            if (d1 != d2) {
                seq_t term2 = phi(d2) * pow_seq_t((seq_t)10, n / d2);
                sum += term2;
            }
        }
    }

    return sum / (seq_t)n;
}
