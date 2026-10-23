/*
 * A040013  Continued fraction for sqrt(18).
 * https://oeis.org/A040013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9454, u16 n <= 9454, u32 n <= 9454, u64 n <= 9454,
 *            u128 n <= 9454, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040013: Continued fraction for sqrt(18)
   Sequence: 4, 4, 8, 4, 8, 4, 8, ...
   Formula: a(n) = 4   when n < 2 or n is odd
            a(n) = 8   otherwise (n even and >= 2)
*/
seq_t a(idx_t n)
{
    /* n is unsigned (uint64_t).  For n<2 we return 4. */
    if (n < 2)
        return (seq_t)4;

    /* For n >= 2: even indices give 8, odd indices give 4. */
    if ((n & (idx_t)1) == 0)
        return (seq_t)8;
    else
        return (seq_t)4;
}
