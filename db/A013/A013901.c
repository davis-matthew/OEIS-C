/*
 * A013901  a(n) = 21^(5*n + 4).
 * https://oeis.org/A013901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(5*n + 4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base value 21 as seq_t */
    seq_t base = (seq_t)21;
    /* exponent = 5*n + 4 */
    idx_t exp = (idx_t)5 * n + 4;

    /* binary exponentiation: result = base^exp */
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
