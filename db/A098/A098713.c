/*
 * A098713  a(n) = (2n+1)*2^(2n+1) - 1.
 * https://oeis.org/A098713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)*2^(2n+1)-1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(2n+1) */
    seq_t pow2 = 1;
    idx_t exp = 2 * n + 1;          /* exponent is non-negative */
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= (seq_t)2;           /* multiply by 2 each step */

    /* multiply by (2n+1) and subtract 1 */
    seq_t mult = (seq_t)(2 * n + 1);
    seq_t result = mult * pow2;
    result -= (seq_t)1;
    return result;
}
