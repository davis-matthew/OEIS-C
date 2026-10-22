/*
 * A001594  a(n) = 6^n + n^6.
 * https://oeis.org/A001594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + n^6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    seq_t m = (seq_t)n;
    seq_t q = m * m * m;
    for (i = 0; i < n; i++)
        p *= 6;
    q = q * q;
    return p + q;
}
