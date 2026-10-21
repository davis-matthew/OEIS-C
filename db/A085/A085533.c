/*
 * A085533  a(n) = (2n)^(2n+1).
 * https://oeis.org/A085533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)^(2n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(2 * n);   /* base = 2n  */
    idx_t   exp  = 2 * n + 1;      /* exponent = 2n+1 */
    seq_t   result = 1;

    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;          /* square base for next bit */
    }

    return result;
}
