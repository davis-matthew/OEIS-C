/*
 * A116138  a(n) = 3^n * n*(n + 1).
 * https://oeis.org/A116138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * n*(n + 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3^n in seq_t */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= (seq_t)3;
    }

    /* Compute n*(n + 1) in seq_t */
    seq_t prod = (seq_t)n * ((seq_t)n + 1);

    /* Final result */
    return pow * prod;
}
