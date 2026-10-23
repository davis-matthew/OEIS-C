/*
 * A158772  a(n) = A138635(n+18)-A138635(n).
 * https://oeis.org/A158772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 21 * 2^floor(n/3)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the exponent e = floor(n/3). */
    idx_t e = n / 3;

    /* Compute 2^e by repeated doubling. */
    seq_t pow = 1;
    for (idx_t i = 0; i < e; ++i)
        pow *= 2;

    /* Multiply by 21 to obtain the term. */
    return pow * 21;
}
