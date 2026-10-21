/*
 * A093141  Expansion of (1-6*x)/(1-10*x).
 * https://oeis.org/A093141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = 4 * 10^(n-1) for n > 0 (OEIS A093141)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
    {
        return (seq_t)1;
    }

    /* Compute 10^(n-1) using exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)10;
    idx_t exp    = n - 1U;   /* n≥1, so exp is safe */

    while (exp > 0U)
    {
        if (exp & 1U)
        {
            result *= base;
        }
        base *= base;
        exp >>= 1U;
    }

    /* Multiply by 4 to get the final term */
    return (seq_t)4 * result;
}
