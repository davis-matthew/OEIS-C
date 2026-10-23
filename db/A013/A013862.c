/*
 * A013862  a(n) = 12^(5*n + 1).
 * https://oeis.org/A013862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 5 * n + 1;          /* exponent as idx_t */
    seq_t result = 1;
    seq_t base   = 12;              /* base of the power */

    while (exp > 0)
    {
        if (exp & 1)
            result *= base;           /* multiply when current bit is 1 */
        if (exp > 1)
            base *= base;             /* square the base for next bit */
        exp >>= 1;                   /* shift exponent right */
    }
    return result;
}
