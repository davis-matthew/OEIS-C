/*
 * A007956  Product of the proper divisors of n.
 * https://oeis.org/A007956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product of proper divisors of n
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 89, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            if (d != n) {
                result *= d;
            }
            idx_t other = n / d;
            if (other != d && other != n) {
                result *= other;
            }
        }
    }
    return result;
}
