/*
 * A098988  Denominators in series expansion of log(Product_{m>=1} (1+q^m)).
 * https://oeis.org/A098988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd( Σ_{d|n} (-1)^(d+1) * (n/d), n )
 * Range:     u8 n <= 254, u16 n <= 9424, u32 n <= 9424, u64 n <= 9424,
 *            u128 n <= 9424, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 by definition */

    int64_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t term1 = (seq_t)n / (seq_t)d1;
            sum += ((d1 & 1) ? 1LL : -1LL) * (int64_t)term1;

            idx_t d2 = n / i;
            if (d1 != d2) {
                seq_t term2 = (seq_t)n / (seq_t)d2;
                sum += ((d2 & 1) ? 1LL : -1LL) * (int64_t)term2;
            }
        }
    }

    seq_t abs_sum = (sum >= 0) ? (seq_t)sum : (seq_t)(-sum);
    seq_t g = gcd(abs_sum, (seq_t)n);
    return ((seq_t)n) / g;
}
