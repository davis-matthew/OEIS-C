/*
 * A065440  a(n) = (n-1)^n.
 * https://oeis.org/A065440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)^n, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0U)
        return (seq_t)1;

    /* (n - 1)^n with fast exponentiation by squaring */
    seq_t base = (seq_t)(n - 1);
    idx_t exp  = n;
    seq_t result = (seq_t)1;

    while (exp)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp)          /* square the base only if more bits remain */
            base *= base;
    }
    return result;
}
