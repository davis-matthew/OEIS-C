/*
 * A192751  Define a pair of sequences c_n, d_n by c_0=0, d_0=1 and thereafter
 *          c_n = c_{n-1}+d_{n-1}, d_n = c_{n-1}+4*n+2; sequence here is c_n.
 * https://oeis.org/A192751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c_n where c_0 = 0, d_0 = 1; for n ≥ 1: c_n = c_{n-1} +
 *            d_{n-1} d_n = c_{n-1} + 4*n + 2 a(n) is the sequence of c_n
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t c = 0;
    seq_t d = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t c_next = c + d;
        seq_t d_next = c + 4 * (seq_t)i + 2;
        c = c_next;
        d = d_next;
    }
    return c;
}
