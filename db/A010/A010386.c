/*
 * A010386  Squares mod 24.
 * https://oeis.org/A010386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = floor((n-1)/6)*24 + [0, 1, 4, 9, 12, 16][(n-1) mod 6]
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n - 1;                 /* shift offset 1 -> index 0 */
    idx_t block = k / 6;
    idx_t pos   = k % 6;
    const seq_t residues[6] = {0, 1, 4, 9, 12, 16};

    /* Each block of 6 numbers corresponds to one complete set of
       quadratic residues modulo 24.  The block is multiplied by 24
       (the modulus) and the appropriate residue is added. */
    return (seq_t)block * 24 + residues[pos];
}
