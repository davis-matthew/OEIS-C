/*
 * A047601  Numbers that are congruent to {0, 1, 3, 4, 5} mod 8.
 * https://oeis.org/A047601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + r[(n-1)%5] where r = {0,1,3,4,5}
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residue_map[5] = {0,1,3,4,5};
    idx_t k = n - 1;            /* zero‑based index */
    idx_t block = k / 5;        /* how many full blocks of 5 */
    idx_t pos   = k % 5;        /* position within the block   */
    seq_t result = ((seq_t)block * (seq_t)8) + residue_map[pos];
    return result;
}
