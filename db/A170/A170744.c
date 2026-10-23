/*
 * A170744  Expansion of g.f.: (1+x)/(1-24*x).
 * https://oeis.org/A170744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 25 * 24^(n-1) for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 1 */
        return (seq_t)1;

    /* Compute 24^(n-1) by binary exponentiation. */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)24;
    idx_t    e    = n - 1;

    while (e)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }

    result *= (seq_t)25;           /* multiply by 25 for a(n) */
    return result;
}
