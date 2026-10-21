/*
 * A050441  Partial sums of A051865.
 * https://oeis.org/A050441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(11*n-8)/6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 821, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t tn  = (seq_t)n;                             /* n                  */
    seq_t tn1 = (seq_t)(n + 1);                       /* n+1                */
    /* 11*n-8 is positive for n >= 1; compute with signed arithmetic first */
    seq_t tn3 = (seq_t)((int64_t)11 * (int64_t)n - 8); /* 11*n-8            */
    return tn * tn1 * tn3 / 6;
}
