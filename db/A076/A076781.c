/*
 * A076781  a(n) = 6^(n^2).
 * https://oeis.org/A076781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent n^2 */
    idx_t exp = (idx_t)n * (idx_t)n;

    /* fast exponentiation: result = base^exp  with base = 6 */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)6;

    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}
