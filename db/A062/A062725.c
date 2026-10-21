/*
 * A062725  Write 0, 1, 2, 3, 4, ... in a triangular spiral, then a(n) is the
 *          sequence found by reading the terms along the line from 0 in the
 *          direction 0, 7, ...
 * https://oeis.org/A062725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n+5)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;               /* cast index to sequence type */
    seq_t part = (seq_t)9 * nt + (seq_t)5;

    /* The product is always even, so we divide an even factor first
       to avoid overflow when seq_t is a fixed-size integer type.   */
    if ((nt & 1) == 0)                 /* nt is even */
        return (nt / 2) * part;
    else                                 /* part is even */
        return nt * (part / 2);
}
