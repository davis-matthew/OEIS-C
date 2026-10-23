/*
 * A047483  Numbers that are congruent to {0, 1, 2, 5, 7} mod 8.
 * https://oeis.org/A047483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {0,1,2,5,7}[(n-1) mod 5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th number congruent to 0,1,2,5, or 7 (mod 8), with a(1)=0. */

static const seq_t _residues[5] = {0, 1, 2, 5, 7};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n>=1 in normal use       */
    idx_t k = n - 1;                 /* zero‑based index                       */
    idx_t block = k / 5;             /* number of full 8‑blocks passed         */
    idx_t pos   = k % 5;             /* position inside the current block      */
    seq_t block8 = ((seq_t)block) << 3;   /* block * 8, computed in seq_t space   */
    return block8 + _residues[pos];
}
