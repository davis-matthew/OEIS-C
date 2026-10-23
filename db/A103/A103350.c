/*
 * A103350  Denominators of sum_{k=1..n} 1/k^8 = Zeta(8,n).
 * https://oeis.org/A103350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of sum_{k=1..n} 1/k^8
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   seq_gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd_local.h"

seq_t a(idx_t n)
{
    seq_t N = 0;   /* current numerator   */
    seq_t D = 1;   /* current denominator */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute k^8 as seq_t, guarding against overflow to zero */
        seq_t k8 = (seq_t)k;
        for (int i = 0; i < 7; ++i)
            k8 *= (seq_t)k;

        if (k8 == 0)            /* term cannot be computed exactly with this seq_t */
            continue;

        /* least common multiple of D and k8 */
        seq_t g = seq_gcd_local(D, k8);
        seq_t newD = (D / g) * k8;

        /* combine the fractions, then reduce */
        seq_t m1 = newD / D;          /* factor to raise N to newD */
        seq_t m2 = newD / k8;         /* factor to raise 1/k8 to newD */
        N = N * m1 + m2;
        D = newD;

        seq_t g2 = seq_gcd_local(N, D);
        N /= g2;
        D /= g2;
    }
    return D;
}
