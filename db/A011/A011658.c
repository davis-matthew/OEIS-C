/*
 * A011658  Period 5: repeat [0, 0, 0, 1, 1]; also expansion of 1/(x^4 + x^3 +
 *          x^2 + x + 1) (mod 2).
 * https://oeis.org/A011658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n mod 5)/3) (period 5 pattern 0,0,0,1,1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 1 for n mod 5 = 3 or 4, otherwise 0. */
    idx_t r = n % 5;
    return (seq_t)(r >= 3 ? 1 : 0);
}
