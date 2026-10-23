/*
 * A013748  a(n) = 11^(3*n + 1).
 * https://oeis.org/A013748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 3 * n + 1;        /* exponent = 3*n + 1 */
    seq_t result = 1;             /* 11^0 */
    seq_t base   = (seq_t)11;     /* 11^1 */

    /* Fast exponentiation by squaring */
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
