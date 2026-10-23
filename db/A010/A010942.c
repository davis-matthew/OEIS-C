/*
 * A010942  Binomial coefficient C(26,n).
 * https://oeis.org/A010942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(26,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 26 the binomial coefficient is zero. */
    if (n > 26u) {
        return (seq_t)0;
    }

    seq_t result = 1;
    /* Compute C(26,n) multiplicatively:
       result = result * (26 - n + i) / i   (exact division) */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * (seq_t)(26 - n + i) / (seq_t)i;
    }

    return result;
}
