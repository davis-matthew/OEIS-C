/*
 * A005005  Davenport-Schinzel numbers of degree n on 4 symbols.
 * https://oeis.org/A005005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1..4) = 1, 4, 7, 12; for n >= 5, a(2*m) = 12*m - 13 and
 *            a(2*m+1) = 12*m - 8 (equivalently a(n) = a(n-1) + a(n-2) -
 *            a(n-3) for n > 7)
 * Range:     u8 n <= 44, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)12;
    idx_t m = n / 2;
    seq_t t = (seq_t)m * 12;
    if (n % 2 == 0) return t - 13;
    return t - 8;
}
