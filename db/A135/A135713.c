/*
 * A135713  a(n) = n*(n+1)*(4*n+1)/2.
 * https://oeis.org/A135713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n+1)/2
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type before any arithmetic */
    seq_t N = (seq_t)n;
    seq_t first  = N * (N + (seq_t)1);   /* n*(n+1) */
    seq_t second = (seq_t)4 * N + (seq_t)1; /* 4*n + 1 */
    seq_t prod   = first * second;        /* n*(n+1)*(4*n+1) */
    return prod / (seq_t)2;               /* divide by 2 */
}
