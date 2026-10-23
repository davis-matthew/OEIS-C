/*
 * A013908  a(n) = 23^(5*n + 3).
 * https://oeis.org/A013908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent = 5*n + 3 */
    idx_t exp = (idx_t)5 * n + (idx_t)3;

    /* fast binary exponentiation */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)23;

    while (exp) {
        if (exp & 1U) result = result * base;
        base = base * base;
        exp >>= 1U;
    }

    return result;
}
