/*
 * A053137  Binomial coefficients C(2*n+8,8).
 * https://oeis.org/A053137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+8, 8)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(2*n+8, 8) multiplicatively to keep intermediate values small.
       The loop multiplies by the numerator then divides by the denominator,
       ensuring each division is exact. */
    seq_t result = 1;
    seq_t base = (seq_t)n * 2 + 8;     /* base = 2*n + 8 */

    for (idx_t i = 1; i <= 8; ++i) {
        seq_t numerator = base + 1 - (seq_t)i; /* base + 1 - i  */
        result = (result * numerator) / (seq_t)i;
    }

    return result;
}
