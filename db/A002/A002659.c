/*
 * A002659  a(n) = 2*sigma(n) - 1.
 * https://oeis.org/A002659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*sigma(n) - 1
 * Range:     u8 n <= 59, u16 n <= 9239, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            if (i != n / i) {
                sigma += (seq_t)(n / i);
            }
        }
    }
    return 2 * sigma - 1;
}
