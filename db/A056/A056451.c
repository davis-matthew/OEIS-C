/*
 * A056451  Number of palindromes using a maximum of five different symbols.
 * https://oeis.org/A056451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^floor((n+1)/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent = floor((n+1)/2) */
    idx_t exp = (n + 1) / 2;
    seq_t result = 1;
    const seq_t five = 5;
    for (idx_t i = 0; i < exp; ++i)
    {
        result *= five;
    }
    return result;
}
