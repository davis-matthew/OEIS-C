/*
 * A168559  a(n) = n^2 + a(n-1), with a(1)=0.
 * https://oeis.org/A168559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)/6 - 1 (sum of squares from 2 to n)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t *= (n + 1);                 /* t = n*(n+1) */
    t *= ((seq_t)2 * n + 1);      /* t = n*(n+1)*(2n+1) */
    t /= 6;                       /* divide by 6, integer division is exact */
    if (t == 0) return 0;         /* handles n = 1, gives 0 without underflow */
    t -= 1;                       /* subtract the 1^2 term */
    return t;
}
