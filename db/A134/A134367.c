/*
 * A134367  a(n) = (n!)^(n-2).
 * https://oeis.org/A134367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)               /* a(0) = a(1) = a(2) = 1 */
        return (seq_t)1;

    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* exponentiate fact to the (n-2) power, using exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = fact;
    idx_t exp    = n - 2;

    while (exp > 0) {
        if (exp & 1ULL)
            result *= base;
        exp >>= 1ULL;
        if (exp > 0)
            base *= base;
    }

    return result;
}
