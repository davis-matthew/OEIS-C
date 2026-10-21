/*
 * A070876  Binary expansion is 1xx100...0 where xx = 00 or 11.
 * https://oeis.org/A070876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9 * 2^(n/2) for n even * a(n) = 15 * 2^((n-1)/2) for n
 *            odd * (equivalently: a(0)=9, a(1)=15, a(n)=2*a(n-2) for n>1)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 57, u64 n <= 121, u128 n <= 249,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* choose the base value according to parity of n */
    seq_t val = (n & 1U) ? 15 : 9;

    /* number of times we must multiply by 2 = floor(n/2) */
    idx_t steps = n >> 1;          /* n / 2 */

    /* multiply by 2 repeatedly – safe for all seq_t types */
    for (idx_t i = 0; i < steps; ++i)
        val <<= 1;                 /* equivalent to val *= 2 */

    return val;
}
