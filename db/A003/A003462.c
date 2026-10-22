/*
 * A003462  a(n) = (3^n - 1)/2.
 * https://oeis.org/A003462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 3 + 1;
    return r;
}
