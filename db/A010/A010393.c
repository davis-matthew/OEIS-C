/*
 * A010393  Squares mod 32.
 * https://oeis.org/A010393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = [0,1,4,9,16,17,25][(n-1) mod 7]
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The residues of squares modulo 32, sorted ascending. */
    static const seq_t residues[7] = {0, 1, 4, 9, 16, 17, 25};

    /* Compute the index in the residue array.  The expression is safe for
       all n ≥ 1 because the offset is 1 and (n-1) is unsigned.           */
    idx_t idx = (n - 1) % 7;

    return residues[idx];
}
