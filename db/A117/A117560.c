/*
 * A117560  a(n) = n*(n^2 - 1)/2 - 1.
 * https://oeis.org/A117560
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n^2 - 1)/2 - 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2047, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We assume n >= 2, as per the OEIS offset.
       For n < 2 the result is not defined, but the
       function will still compute a mathematically
       equivalent value. */
    seq_t N = n;
    seq_t n2 = N * N;                     /* n^2 */
    seq_t prod = N * (n2 - (seq_t)1);      /* n*(n^2 – 1) */
    seq_t res = prod / (seq_t)2;           /* divide by 2 */
    res -= (seq_t)1;                       /* subtract the final 1 */
    return res;
}
