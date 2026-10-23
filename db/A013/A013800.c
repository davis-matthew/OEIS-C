/*
 * A013800  a(n) = 14^(4*n + 1).
 * https://oeis.org/A013800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the exponent e = 4*n + 1 as a normal integer */
    idx_t e = n * 4 + 1;

    /* Prepare base and result as seq_t */
    seq_t base   = (seq_t)14;
    seq_t result = (seq_t)1;

    /* Exponentiation by squaring */
    while (e > 0)
    {
        if (e & 1)
            result *= base;     /* multiply result by base if exponent bit is set */
        base *= base;           /* square the base */
        e >>= 1;                /* shift exponent right */
    }

    return result;
}
