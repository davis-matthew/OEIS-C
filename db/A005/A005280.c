/*
 * A005280  Davenport-Schinzel numbers of degree 5 on n symbols.
 * https://oeis.org/A005280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (3n^2 + 24n - 19)/8 if n is odd; a(n) = (n^2 + 16n -
 *            16)/4 if n is even
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (n & 1) {
        return (3 * sn * sn + 24 * sn - 19) / 8;
    } else {
        return (sn * sn + 16 * sn - 16) / 4;
    }
}
