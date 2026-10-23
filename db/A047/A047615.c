/*
 * A047615  Numbers that are congruent to {0, 5} mod 8.
 * https://oeis.org/A047615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n - 7 + (-1)^n)/2
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t so all arithmetic stays in seq_t domain */
    seq_t nn = (seq_t)n;

    /* Compute 8*n - 7 */
    seq_t term = 8 * nn - (seq_t)7;

    /* Add or subtract 1 depending on parity of n */
    if ((n & (idx_t)1) == 0)
        term += (seq_t)1;
    else
        term -= (seq_t)1;

    /* Final division by 2 */
    return term / (seq_t)2;
}
