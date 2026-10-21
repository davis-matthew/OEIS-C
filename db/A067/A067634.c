/*
 * A067634  a(1) = 1; for n > 1, a(n) is the least integer > a(n-1) such that
 *          the string of digits of a(n-1)^2 is a substring of the string of
 *          digits of a(n)^2.
 * https://oeis.org/A067634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=4, a(n)=13·10^(n-3) for n≥3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    if (n == 2)
        return 4;

    /* n ≥ 3 */
    idx_t exp = n - 3;          /* exponent of 10 */
    seq_t result = 13;          /* start with the multiplier */
    seq_t base = 10;            /* current power of 10 */

    /* fast exponentiation by squaring */
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}
