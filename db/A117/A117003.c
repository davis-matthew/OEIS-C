/*
 * A117003  a(n) = sigma(n) + A079667(n).
 * https://oeis.org/A117003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n} max(d, n/d)
 * Range:     u8 n <= 59, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (d == e) {
                total += (seq_t)d;           /* sqrt(n) contributes once */
            } else {
                total += (seq_t)e * 2;       /* both divisors contribute e */
            }
        }
    }
    return total;
}
