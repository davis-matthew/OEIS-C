/*
 * A013755  a(n) = 14^(3*n + 2).
 * https://oeis.org/A013755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14^(3*n+2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)2744;  /* 14^3 = 2744 */
    seq_t result = (seq_t)1;
    idx_t exp    = n;

    /* binary exponentiation: result = base^exp */
    while (exp) {
        if (exp & 1u)   result *= base;
        base *= base;
        exp >>= 1u;
    }

    /* a(n) = 196 * 2744^n = 14^2 * 14^(3n) = 14^(3n+2) */
    return result * (seq_t)196;
}
