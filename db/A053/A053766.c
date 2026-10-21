/*
 * A053766  a(n) = 5^(n^2 - n).
 * https://oeis.org/A053766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^(n^2 - n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n^2 - n = n*(n-1) */
    if (n < 2)            /* exponent 0 -> 5^0 = 1 */
        return 1;

    idx_t exp = (idx_t)n * ((idx_t)n - 1);   /* exponent */
    seq_t result = 1;
    seq_t base   = 5;

    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp  >>= 1;
    }

    return result;
}
