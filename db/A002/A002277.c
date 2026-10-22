/*
 * A002277  a(n) = 3*(10^n - 1)/9.
 * https://oeis.org/A002277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + 3, a(0) = 0 (equivalently a(n) = 3*(10^n -
 *            1)/9)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 10 + 3;
    return r;
}
