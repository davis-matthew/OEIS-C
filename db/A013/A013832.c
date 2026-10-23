/*
 * A013832  a(n) = 4^(5*n + 3).
 * https://oeis.org/A013832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(5*n + 3)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = 5 * n + 3;          /* exponent 5*n+3 */
    seq_t result = 1;
    seq_t base   = (seq_t)4;        /* base 4 */

    /* exponentiation by squaring (works for any seq_t type) */
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
