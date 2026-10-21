/*
 * A072345  Volume of n-dimensional sphere of radius r is V_n*r^n =
 *          Pi^(n/2)*r^n/(n/2)! = C_n*Pi^floor(n/2)*r^n; sequence gives
 *          numerator of C_n.
 * https://oeis.org/A072345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1 if n even, 2^((n+1)/2) if n odd
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1U) == 0U)        /* even index */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t exp = (n + 1U) / 2U; /* exponent for power of two */

    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;    /* multiply by 2 each step */

    return result;
}
