/*
 * A095033  An example of a (v,k,lambda)=(19,9,4) cyclic difference set.
 * https://oeis.org/A095033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 19*⌊(n-1)/9⌋ + {1,4,5,6,7,9,11,16,17}[(n-1)%9]
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A095033: numbers congruent to 1,4,5,6,7,9,11,16,17 modulo 19. */
seq_t a(idx_t n)
{
    /* Residue pattern for the (19,9,4) difference set. */
    static const idx_t residues[9] = {1, 4, 5, 6, 7, 9, 11, 16, 17};

    idx_t block = (n - 1) / 9;      /* zero‑based period number */
    idx_t pos   = (n - 1) % 9;      /* position within the period */

    return ((seq_t)block) * (seq_t)19 + residues[pos];
}
