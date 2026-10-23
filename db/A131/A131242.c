/*
 * A131242  Partial sums of A059995: a(n) = Sum_{k=0..n} floor(k/10).
 * https://oeis.org/A131242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*floor(n/10)*(2n-8-10*floor(n/10))
 * Range:     u8 n <= 54, u16 n <= 816, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t t = N / 10;            /* floor(n/10) */
    seq_t inner = 2 * N - (seq_t)8 - 10 * t; /* 2n-8-10*floor(n/10) */
    seq_t prod  = t * inner;     /* floor(n/10)*(...) */
    return prod / 2;             /* divide by 2 */
}
