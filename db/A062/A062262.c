/*
 * A062262  Fifth (unsigned) column sequence of triangle A062140 (generalized
 *          a=4 Laguerre).
 * https://oeis.org/A062262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+4)! * binomial(n+8, 8)) / 24
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute binomial(n+8,8) as product / 8! */
    seq_t bin = 1;
    idx_t top = n + 8;
    for (idx_t i = 0; i < 8; ++i) {
        bin *= (seq_t)(top - i);          /* multiply by (n+8-i) */
    }
    /* divide by 8! = 40320 (exact division) */
    bin /= 40320;                         /* 8! == 40320 */

    /* compute product of integers 5 .. n+4 (empty if n == 0) */
    seq_t prod = 1;
    for (idx_t i = 5; i <= n + 4; ++i) {
        prod *= (seq_t)i;
    }

    /* final result */
    return prod * bin;
}
