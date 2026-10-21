/*
 * A091311  Partial sums of 3^A007814(n).
 * https://oeis.org/A091311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1 a(2n) = 3*a(n) + n a(2n+1) = 3*a(n) + n + 1
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;          /* a(0)=0, a(1)=1 */

    idx_t m = n >> 1;            /* m = floor(n/2) */
    seq_t half = a(m);           /* recursive call on the smaller index */

    if ((n & 1) == 0)            /* even: n = 2m */
        return half * (seq_t)3 + (seq_t)m;
    else                         /* odd:  n = 2m+1 */
        return half * (seq_t)3 + (seq_t)m + 1;
}
