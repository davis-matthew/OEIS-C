/*
 * A097110  Expansion of (1 + 2x - 2x^3) / (1 - 3x^2 + 2x^4).
 * https://oeis.org/A097110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor((n+2)/2) - 1 + (n mod 2)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute term a(n) of A097110 */
seq_t a(idx_t n)
{
    /* exponent of 2 */
    idx_t exp = (n + 2) / 2;               /* floor((n+2)/2) */

    /* 2^exp as a seq_t value (shift is defined for integral seq_t types
       and for big integer types that support the << operator). */
    seq_t power_of_two = (seq_t)1 << exp;   /* 1 * 2^exp */

    /* combine into the final expression */
    seq_t result = power_of_two - 1 + (seq_t)(n & 1);

    return result;
}
