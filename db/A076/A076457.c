/*
 * A076457  Sum of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly four ways.
 * https://oeis.org/A076457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(8*n^2+6*n-1)/2
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; return 0 for n==0 to keep the function safe. */
    if (n == 0) return 0;

    seq_t sn   = (seq_t)n;            /* convert idx_t to seq_t */
    seq_t sn1  = sn + (seq_t)1;       /* n + 1 */
    seq_t n2   = sn * sn;             /* n^2 */

    seq_t t1   = n2 * (seq_t)8;       /* 8*n^2 */
    seq_t t2   = sn * (seq_t)6;       /* 6*n  */
    seq_t inner = t1 + t2 - (seq_t)1; /* 8*n^2 + 6*n - 1 */

    seq_t prod = sn * sn1;            /* n*(n+1) */
    prod = prod * inner;              /* n*(n+1)*(8*n^2+6*n-1) */
    prod = prod / (seq_t)2;           /* final division by 2 */

    return prod;
}
