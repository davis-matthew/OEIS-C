/*
 * A185699  Expansion of (11 * E_2(x^11) - E_2(x)) / 2 in powers of x where
 *          E_2() is an Eisenstein series.
 * https://oeis.org/A185699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; for n > 0, a(n) = 12 * sum_{d|n, gcd(d,11)=1} d (i.e.
 *            sum of all divisors of n not divisible by 11)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (d % 11 != 0) sum += (seq_t)d;
            /* complementary divisor n/d, avoid double counting when d==n/d */
            idx_t e = n / d;
            if (e != d && e % 11 != 0) sum += (seq_t)e;
        }
    }
    return sum * (seq_t)12;
}
