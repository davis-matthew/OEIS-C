/*
 * A013756  a(n) = 15^(3*n + 1).
 * https://oeis.org/A013756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t exp = ((seq_t)n * 3) + 1; /* exponent = 3*n + 1 */
    seq_t result = 1;
    seq_t base   = 15;

    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}
