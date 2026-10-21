/*
 * A083420  a(n) = 2*4^n - 1.
 * https://oeis.org/A083420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*4^n - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4^n using binary exponentiation */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    while (n != 0)
    {
        if (n & (idx_t)1LL)
            result *= base;
        base *= base;
        n >>= 1;
    }
    /* Multiply by 2 and subtract 1 */
    result *= (seq_t)2;
    result -= (seq_t)1;
    return result;
}
