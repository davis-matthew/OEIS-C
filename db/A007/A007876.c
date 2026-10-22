/*
 * A007876  a(2n-1) = n*a(2n-2), a(2n) = n*a(2n-1) + 1.
 * https://oeis.org/A007876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = n*a(2n-2), a(2n) = n*a(2n-1) + 1, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t k = (seq_t)(i % 2 == 0 ? i / 2 : (i + 1) / 2);
        result = k * result;
        if (i % 2 == 0) result += 1;
    }
    return result;
}
