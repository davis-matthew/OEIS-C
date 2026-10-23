/*
 * A024212  2nd elementary symmetric function of first n+1 positive integers
 *          congruent to 1 mod 3.
 * https://oeis.org/A024212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(9*n^2+9*n-2)/8
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 247, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* t = n*(n+1)/2  (avoids overflow for n up to about 10^4) */
    seq_t t = nn * (nn + 1) / 2;

    /* u = 9*n^2 + 9*n - 2  */
    seq_t u = (seq_t)9 * nn * nn + (seq_t)9 * nn - (seq_t)2;

    /* result = t * u / 4  */
    return t * u / (seq_t)4;
}
