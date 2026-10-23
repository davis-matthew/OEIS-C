/*
 * A026040  a(n) = dot_product(1,2,...,n)*(4,5,...,n,1,2,3).
 * https://oeis.org/A026040
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - 3*n + 14)/3
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t tmp     = n_seq * n_seq;   /* n^2 */
    tmp = tmp - (seq_t)3 * n_seq;    /* n^2 - 3*n */
    tmp = tmp + 14;                  /* n^2 - 3*n + 14 */
    return (n_seq * tmp) / (seq_t)3; /* n * (...) / 3 */
}
