/*
 * A013718  a(n) = 13^(2*n + 1).
 * https://oeis.org/A013718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 13 * 169^n using exponentiation by squaring */
    seq_t result = (seq_t)13;
    seq_t base   = (seq_t)169;
    idx_t e = n;

    while (e > 0)
    {
        if (e & 1U)
            result *= base;      /* multiply result by current base */
        base *= base;            /* square the base */
        e >>= 1U;                /* halve the exponent */
    }
    return result;
}
