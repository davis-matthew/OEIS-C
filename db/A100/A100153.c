/*
 * A100153  Structured truncated dodecahedral numbers.
 * https://oeis.org/A100153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*n*(33*n^2 - 41*n + 10)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 604, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first */
    seq_t n_seq = (seq_t)n;
    /* n^2 */
    seq_t n_sq  = n_seq * n_seq;
    /* 33*n^2 - 41*n + 10 */
    seq_t inner = (33 * n_sq) - (41 * n_seq) + (seq_t)10;
    /* n * inner */
    seq_t prod  = n_seq * inner;
    /* divide by 2 – the result is always an integer */
    return prod / (seq_t)2;
}
