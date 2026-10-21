/*
 * A095036  An example of a (v,k,lambda)=(91,10,1) cyclic difference set.
 * https://oeis.org/A095036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 91 * floor((n-1)/10) + d[(n-1) mod 10] where d = [0, 2,
 *            6, 7, 18, 21, 31, 54, 63, 71]
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The set of residues that form a cyclic (91,10,1) difference set. */
    static const idx_t residues[10] = {
        0, 2, 6, 7, 18, 21, 31, 54, 63, 71
    };

    /* Zero‑based block and within‑block index. */
    idx_t block = (n - 1) / 10;
    idx_t pos   = (n - 1) % 10;

    /* Compute 91 * block + residue. Use seq_t for arithmetic. */
    seq_t val = (seq_t)block * ((seq_t)91);
    val += (seq_t)residues[pos];

    return val;
}
