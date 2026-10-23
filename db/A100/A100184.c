/*
 * A100184  Structured octagonal anti-prism numbers.
 * https://oeis.org/A100184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(19*n^3 - 15*n^2 + 2*n)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 1106, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type */
    seq_t n_t = (seq_t)n;
    /* Compute n^2 and n^3 */
    seq_t n_sq = n_t * n_t;
    seq_t n_cube = n_sq * n_t;
    /* Compute the polynomial 19*n^3 - 15*n^2 + 2*n */
    seq_t term = (seq_t)19 * n_cube
               - (seq_t)15 * n_sq
               + (seq_t)2 * n_t;
    /* Divide by 6 (the expression is always divisible by 6) */
    return term / 6;
}
