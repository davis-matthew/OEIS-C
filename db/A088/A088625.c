/*
 * A088625  a(n) = 14*binomial(n,8).
 * https://oeis.org/A088625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14 * binomial(n,8)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 8 the binomial coefficient is zero */
    if (n < 8) {
        return 0;
    }

    seq_t result = 1;
    /* Compute C(n,8) multiplicatively to avoid overflow of factorials.
       At each step the division is exact. */
    for (idx_t i = 1; i <= 8; ++i) {
        seq_t numerator = (seq_t)n - (seq_t)(8 - i);
        result = (result * numerator) / (seq_t)i;
    }

    /* Multiply by 14 */
    return result * (seq_t)14;
}
