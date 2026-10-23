/*
 * A174723  a(n) = n*(4*n^2 - 3*n + 5)/6.
 * https://oeis.org/A174723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2 - 3*n + 5)/6
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                    /* cast index to seq_t */
    /* compute 4*n^2 - 3*n + 5 */
    seq_t w = ((seq_t)4 * nn * nn) - ((seq_t)3 * nn) + (seq_t)5;
    /* final formula */
    return (nn * w) / (seq_t)6;
}
