/*
 * A047404  Numbers that are congruent to {1, 2, 3, 6} mod 8.
 * https://oeis.org/A047404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 8 * floor((n-1)/4) + offset[(n-1)%4] * where
 *            offset[0]=1, offset[1]=2, offset[2]=3, offset[3]=6 * This
 *            generates numbers congruent to 1,2,3,6 modulo 8 with offset 1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the block index and position within the block */
    idx_t block = (n - 1) / 4;
    idx_t pos   = (n - 1) % 4;
    /* offsets for the four accepted residues */
    static const seq_t off[4] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)6 };
    /* result: 8*block plus the offset for the position */
    return ((seq_t)8 * (seq_t)block) + off[pos];
}
