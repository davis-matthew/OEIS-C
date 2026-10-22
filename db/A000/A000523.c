/*
 * A000523  a(n) = floor(log_2(n)).
 * https://oeis.org/A000523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look
 * Formula:   a(n) = floor(log_2(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        result++;
    }
    return result;
}
