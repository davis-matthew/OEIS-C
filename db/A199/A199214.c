/*
 * A199214  a(n) = 3*5^n+1.
 * https://oeis.org/A199214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*5^n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result   = (seq_t)1;   /* pow = 5^n  */
    seq_t base     = (seq_t)5;
    idx_t exp      = n;

    while (exp)
    {
        if (exp & 1u)        /* if lowest bit is set */
            result *= base; /* result *= base */
        base *= base;        /* base *= base  */
        exp >>= 1;           /* exp >>= 1    */
    }
    return result * (seq_t)3 + (seq_t)1;
}
