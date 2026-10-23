/*
 * A048490  a(n) = T(7,n), array T given by A048483.
 * https://oeis.org/A048490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * 2^n - 7 * This is equivalent to: a(0)=1,
 *            a(n)=2*a(n-1)+7 for n>0. * The implementation builds 2^n by
 *            repeated left–shift, which works * for any unsigned
 *            implementation of seq_t (fixed‑width or arbitrary * precision).
 *            All arithmetic uses only the allowed operators
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = (seq_t)1;          /* 2^0 */
    for (idx_t i = 0; i < n; ++i)
    {
        pow2 <<= 1;                 /* multiply by 2 */
    }
    pow2 *= (seq_t)8;                /* 8 * 2^n */
    pow2 -= (seq_t)7;                /* subtract 7 */
    return pow2;
}
