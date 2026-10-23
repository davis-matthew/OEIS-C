/*
 * A047388  Numbers that are congruent to {0, 1, 2, 5} mod 7.
 * https://oeis.org/A047388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/4)*7 + [0,1,2,5][(n-1)%4]
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0 <= n, n>=1  */
seq_t a(idx_t n)
{
    static const seq_t residues[4] = {0, 1, 2, 5};
    /* n is 1-indexed; all calculations are done in unsigned types */
    idx_t k = (n - 1) / 4;          /* number of full blocks of 7 */
    idx_t r = (n - 1) % 4;          /* position within the block */
    return ((seq_t)k) * 7 + residues[r];
}
