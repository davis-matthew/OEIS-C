/*
 * A178910  Binary XOR of divisors of n.
 * https://oeis.org/A178910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = XOR of all positive divisors of n
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            result ^= (seq_t)i;
            idx_t j = n / i;
            if (j != i) {
                result ^= (seq_t)j;
            }
        }
    }
    return result;
}
