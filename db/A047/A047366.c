/*
 * A047366  Numbers that are congruent to {1, 3, 4, 5} mod 7.
 * https://oeis.org/A047366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + {1,3,4,5}[ (n-1)%4 ]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute block number and offset in block */
    idx_t m = (n - 1) / 4;          /* number of complete allowed blocks before n */
    idx_t r = (n - 1) % 4;          /* position within the current block (0..3) */

    /* Allowed residues within each 7‑block, in order */
    static const seq_t residues[4] = { (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)5 };

    /* Compute the value: 7*block + residue */
    seq_t base = (seq_t)7 * m;
    return base + residues[r];
}
