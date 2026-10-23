/*
 * A106352  Number of compositions of n into 3 parts such that no two adjacent
 *          parts are equal.
 * https://oeis.org/A106352
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)/2 - 2*floor((n-1)/2) + (n%3==0 ? 1 : 0)
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;                /* values below the offset are undefined */

    seq_t t = ((seq_t)(n - 1) * (n - 2)) / 2;  /* choose(n-1,2) */
    seq_t f = (seq_t)((n - 1) / 2);            /* floor((n-1)/2) */

    seq_t res = t - 2 * f;                     /* subtract compositions with a=b or b=c */
    if (n % 3 == 0) res += (seq_t)1;           /* add back the a=b=c case */

    return res;
}
