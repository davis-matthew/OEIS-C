/*
 * A179783  a(n) = 2*n*(n+1)*(n+2)/3 + (-1)^n.
 * https://oeis.org/A179783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n+1)*(n+2)/3 + (-1)^n
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1858, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the cubic term in seq_t and add or subtract 1 depending on parity */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* compute 2*n*(n+1)*(n+2)/3 */
    seq_t t = (seq_t)2 * n_seq * (n_seq + 1) * (n_seq + 2);
    seq_t pos = t / (seq_t)3;
    /* add (-1)^n: +1 if n even, -1 if n odd */
    return (n & (idx_t)1) ? pos - (seq_t)1 : pos + (seq_t)1;
}
