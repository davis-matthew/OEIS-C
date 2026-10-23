/*
 * A198687  a(n) = 5*7^n-1.
 * https://oeis.org/A198687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*7^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t nn = n;          /* use a copy of n for bit manipulation */
    seq_t result = 1;      /* holds 7^n during exponentiation */
    seq_t base   = 7;      /* base of the exponentiation */

    /* fast exponentiation: result *= base whenever the current bit is 1 */
    while (nn) {
        if (nn & 1) {
            result *= base;
        }
        base *= base;
        nn >>= 1;
    }

    /* apply the final multipliers of the formula */
    result *= 5;
    result -= 1;
    return result;
}
