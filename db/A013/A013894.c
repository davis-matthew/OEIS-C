/*
 * A013894  a(n) = 20^(5*n + 1).
 * https://oeis.org/A013894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^(5*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 20;                    /* 20^1 */
    idx_t exp = 5 * n + 1;               /* 5*n+1 */

    /* fast exponentiation by squaring, works for any seq_t type */
    while (exp)
    {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;     /* avoid a useless multiply after loop ends */
    }
    return result;
}
