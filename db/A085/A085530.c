/*
 * A085530  a(n) = (2n+1)^(2n).
 * https://oeis.org/A085530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)^(2n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (2n+1) in seq_t */
    seq_t base = (seq_t) n * 2 + 1;
    /* Exponent is 2n, fits in idx_t */
    idx_t exp = n * 2;
    /* Fast binary exponentiation */
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
