/*
 * A017520  a(n) = (11*n + 10)^12.
 * https://oeis.org/A017520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base value (11*n + 10) */
    seq_t base = (seq_t)n * 11 + 10;
    seq_t result = 1;
    seq_t pow_base = base;
    int exp = 12;          /* exponent is fixed for this sequence */

    /* binary exponentiation */
    while (exp > 0) {
        if (exp & 1)
            result *= pow_base;
        pow_base *= pow_base;
        exp >>= 1;
    }

    return result;
}
