/*
 * A224880  a(n) = 2*n + sum of divisors of n.
 * https://oeis.org/A224880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + sigma(n)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            if (i != n / i) {
                sum += (seq_t)(n / i);
            }
        }
    }
    seq_t nn = (seq_t)n;
    return (seq_t)2 * nn + sum;
}
