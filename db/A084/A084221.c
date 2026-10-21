/*
 * A084221  a(n+2) = 4*a(n), with a(0)=1, a(1)=3.
 * https://oeis.org/A084221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 4*a(n), a(0) = 1, a(1) = 3 (equivalently a(n) = (5*2^n
 *            - (-2)^n)/4)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* compute 2^n as pow2 */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;      /* unsigned shift: 2^i each iteration */

    /* even n → a(n) = 2^n
       odd n  → a(n) = 3*2^(n-1) = 3*(pow2 >> 1) */
    if ((n & 1) == 0)
        return pow2;
    else
        return (pow2 >> 1) * (seq_t)3;
}
