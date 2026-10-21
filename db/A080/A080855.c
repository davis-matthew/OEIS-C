/*
 * A080855  a(n) = (9*n^2 - 3*n + 2)/2.
 * https://oeis.org/A080855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 - 3*n + 2)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t to avoid any integer overflow on smaller types. */
    seq_t nn = (seq_t)n;
    /* Compute the expression using only seq_t arithmetic. */
    return ( ( (seq_t)9 * nn * nn ) - ( (seq_t)3 * nn ) + (seq_t)2 ) / (seq_t)2;
}
