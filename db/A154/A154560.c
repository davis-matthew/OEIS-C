/*
 * A154560  a(n) = (n+3)^2*n/2 + 1.
 * https://oeis.org/A154560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 3)^2 * n / 2 + 1 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1926, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert index to seq_t */
    seq_t t  = nn + (seq_t)3;            /* (n + 3)         */
    seq_t sq = t * t;                    /* (n + 3)^2       */
    seq_t prod = sq * nn;                /* n * (n + 3)^2   */
    prod = prod / (seq_t)2;              /* divide by 2      */
    prod = prod + (seq_t)1;              /* add 1            */
    return prod;
}
