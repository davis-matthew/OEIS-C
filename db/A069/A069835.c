/*
 * A069835  Define an array as follows: b(i,0)=b(0,j)=1, b(i,j) = 2*b(i-1,j-1)
 *          + b(i-1,j) + b(i,j-1). Then a(n) = b(n,n).
 * https://oeis.org/A069835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binom(n, k)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;   /* running total */
    seq_t binom = 1;   /* binom(n, 0) */
    seq_t term3 = 1;   /* 3^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add term = 3^k * C(n,k)^2 */
        sum += term3 * (binom * binom);

        /* advance to next k */
        if (k == n) break;
        binom = (binom * (n - k)) / (k + 1);
        term3 *= 3;
    }

    return sum;
}
