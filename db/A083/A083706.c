/*
 * A083706  a(n) = 2^(n+1) + n - 1.
 * https://oeis.org/A083706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + n - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 2;
    idx_t exp    = n;
    exp += 1U;                          /* exponent = n + 1 */
    for (; exp > 0; exp >>= 1)
    {
        if (exp & 1U)
            result *= base;           /* multiply by current power of 2 */
        base *= base;                 /* square the base */
    }
    result += (seq_t)n;                /* add n */
    result -= 1;                       /* subtract 1 */
    return result;
}
