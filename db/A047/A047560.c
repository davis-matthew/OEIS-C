/*
 * A047560  Numbers that are congruent to {0, 2, 3, 6, 7} mod 8.
 * https://oeis.org/A047560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {0,2,3,6,7}[(n-1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047560: Numbers congruent to {0,2,3,6,7} modulo 8. */

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t idx = n - 1;
    idx_t block = idx / 5;
    idx_t pos   = idx % 5;
    static const seq_t residues[5] = {0, 2, 3, 6, 7};
    return ((seq_t)block) * (seq_t)8 + residues[pos];
}
