/*
 * A133622  a(n) = 1 if n is odd, a(n) = n/2+1 if n is even.
 * https://oeis.org/A133622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is odd, a(n) = n/2 + 1 if n is even
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined by definition, guard against accidental use */
    if ((n & 1) != 0)
        return 1;           /* odd n */
    return ((seq_t)n) / 2 + 1;   /* even n */
}
