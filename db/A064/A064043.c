/*
 * A064043  Number of length 3 walks on an n-dimensional hypercubic lattice
 *          starting at the origin and staying in the nonnegative part.
 * https://oeis.org/A064043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 3*n - 1)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;              /* cast index to seq_t */
    seq_t n2 = x * x;                /* n^2 */
    seq_t term = n2 + 3 * x - 1;     /* n^2 + 3*n - 1 */
    return x * term;                 /* n * (n^2 + 3*n - 1) */
}
