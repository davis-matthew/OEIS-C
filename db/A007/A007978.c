/*
 * A007978  Least non-divisor of n.
 * https://oeis.org/A007978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Least k >= 2 such that k does not divide n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    for (seq_t k = 2; ; k++) {
        if (n % (idx_t)k != 0) {
            return k;
        }
    }
}
