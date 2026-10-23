/*
 * A160250  a(n) = 64*n^3 - 168*n^2 + 148*n - 43.
 * https://oeis.org/A160250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 64*n^3 - 168*n^2 + 148*n - 43
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 407, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Perform the computation in stages to avoid overflow on unsigned
       types smaller than 64 bits for values that actually fit. */
    seq_t term = ((64 * nn - 168) * nn + 148) * nn - 43;
    return term;
}
