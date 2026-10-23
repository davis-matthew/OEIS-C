/*
 * A010380  Squares mod 18.
 * https://oeis.org/A010380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = sorted distinct residues of squares modulo 18, repeated
 *            with a period of 8. The residues are: 0, 1, 4, 7, 9, 10, 13, 16
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precomputed residues */
    static const seq_t residues[8] = {0, 1, 4, 7, 9, 10, 13, 16};
    /* (n-1) % 8 gives the index in the periodic array */
    return residues[(idx_t)((n - 1) % 8)];
}
