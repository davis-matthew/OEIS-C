/*
 * A195147  Concentric 18-gonal numbers.
 * https://oeis.org/A195147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n^2 + 7*(-1)^n - 7)/4 (equivalently a(n) = (9*n^2 +
 *            (-7)*(n%2))/2)
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2    = n_seq * n_seq;
    seq_t val   = 9 * n2;          /* 9*n^2 */
    if (n & 1) val -= 7;            /* subtract 7 when n is odd */
    return val / 2;                 /* divide by 2 gives the final result */
}
