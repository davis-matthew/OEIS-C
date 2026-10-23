/*
 * A199212  a(n) = 2*5^n+1.
 * https://oeis.org/A199212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*5^n+1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;   /* 5^0 */
    seq_t base   = 5;   /* current power of 5 */

    idx_t exp = n;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;          /* multiply only when needed */
        base *= base;                 /* square the base for next bit */
        exp >>= 1;
    }

    /* a(n) = 2 * 5^n + 1 */
    result = result * 2 + 1;
    return result;
}
