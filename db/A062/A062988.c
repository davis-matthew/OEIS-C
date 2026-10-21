/*
 * A062988  a(n) = binomial(n+6,5) - 1.
 * https://oeis.org/A062988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,5) - 1
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 163, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes C(n+6,5) using the multiplicative formula to avoid large intermediate values.
 * The implementation is compatible with any seq_t/idx_t type provided by the build system. */
seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        val = val * (((seq_t)n) + 1 + i);
        val = val / (seq_t)i;
    }
    return val - 1;
}
