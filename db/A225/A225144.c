/*
 * A225144  a(n) = Sum_{i=n..2*n} i^2*(-1)^i.
 * https://oeis.org/A225144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n+1)/2 if n is even; a(n) = 3*n*(n+1)/2 if n is odd
 * Range:     u8 n <= 11, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The value can be computed in O(1) time. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    if ((n & 1) == 0)          /* n is even */
        return n_seq * (((seq_t)5 * n_seq) + 1) / 2;
    else                       /* n is odd */
        return 3 * n_seq * (n_seq + 1) / 2;
}
