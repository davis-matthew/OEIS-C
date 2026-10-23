/*
 * A152917  A000169 prefixed by an initial 0.
 * https://oeis.org/A152917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 0, otherwise a(n) = n^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t base   = (seq_t)n;     /* n */
    idx_t exp    = n - 1;        /* exponent n-1 */
    seq_t result = (seq_t)1;     /* multiplicative identity */

    /* binary exponentiation */
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}
