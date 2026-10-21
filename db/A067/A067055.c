/*
 * A067055  a(n) = (n!)^(n*(n+1)/2).
 * https://oeis.org/A067055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!)^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0 || n == 1) return (seq_t)1;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute exponent k = n*(n+1)/2 */
    unsigned long long k = (unsigned long long)n * (n + 1u) / 2u;

    /* Power by exponentiation by squaring */
    seq_t result = 1;
    seq_t base = fact;
    while (k > 0)
    {
        if (k & 1u)
            result *= base;
        base *= base;
        k >>= 1;
    }
    return result;
}
