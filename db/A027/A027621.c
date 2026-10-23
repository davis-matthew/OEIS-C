/*
 * A027621  a(n) = n + (n+1)^2 + (n+2)^3 + (n+3)^4.
 * https://oeis.org/A027621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)^2 * (n^2 + 7*n + 10)
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 252, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n3 = nn + (seq_t)3;
    seq_t n_sq = nn * nn;
    seq_t poly = n_sq + (seq_t)7 * nn + (seq_t)10;
    return n3 * n3 * poly;
}
