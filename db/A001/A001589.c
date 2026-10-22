/*
 * A001589  a(n) = 4^n + n^4.
 * https://oeis.org/A001589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + n^4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    seq_t m = (seq_t)n;
    for (i = 0; i < n; i++)
        p *= 4;
    return p + m * m * m * m;
}
