/*
 * A067041  a(n) = n^(product of digits of n).
 * https://oeis.org/A067041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^(product of the decimal digits of n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 17, u64 n <= 26, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is a special case: a(0) is defined as 1 (0^0 = 1) */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the product of the decimal digits of n */
    idx_t prod = 1;
    idx_t tmp = n;
    while (tmp > 0) {
        idx_t digit = tmp % 10;
        prod *= digit;
        tmp /= 10;
    }

    /* Fast exponentiation by squaring: result = base^prod */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)n;

    while (prod > 0) {
        if (prod & 1) {
            result *= base;
        }
        prod >>= 1;
        if (prod) {
            base *= base;
        }
    }

    return result;
}
