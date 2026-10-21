/*
 * A078181  a(n) = Sum_{d|n, d == 1 (mod 3)} d.
 * https://oeis.org/A078181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n, d == 1 (mod 3)} d
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* enumerate divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            if (i % 3 == 1) {
                sum += (seq_t)i;
            }
            /* divisor d2 = n / i */
            idx_t j = n / i;
            if (j != i && j % 3 == 1) {
                sum += (seq_t)j;
            }
        }
    }
    return sum;
}
