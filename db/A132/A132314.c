/*
 * A132314  a(n) = n*2^floor((n+1)/2).
 * https://oeis.org/A132314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^floor((n+1)/2)
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 52, u64 n <= 114, u128 n <= 240,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute t = floor((n+1)/2) */
    idx_t t = (n + 1) >> 1;

    /* compute 2^t as a seq_t value */
    seq_t two_pow = (seq_t)1;
    for (idx_t i = 0; i < t; ++i)
        two_pow <<= 1;

    /* (seq_t)n * 2^t is the desired term */
    return (seq_t)n * two_pow;
}
