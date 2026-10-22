/*
 * A006044  a(n) = 4^(n-4)*(n-1)*(n-2)*(n-3).
 * https://oeis.org/A006044
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-4)*(n-1)*(n-2)*(n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t r = (m - 1) * (m - 2) * (m - 3);
    idx_t i;
    for (i = 4; i < n; i++)
        r *= 4;
    return r;
}
