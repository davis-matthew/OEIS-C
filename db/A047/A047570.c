/*
 * A047570  Numbers that are congruent to {2, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/5) + {2,4,5,6,7}[(n-1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047570: numbers congruent to {2,4,5,6,7} mod 8 */
seq_t a(idx_t n)
{
    /* offset is 1, so n should be >= 1 (behaviour for n==0 is undefined) */
    if (n == 0) return 0;                      /* safety; not part of the sequence */

    /* divide into complete blocks of 5 accepted numbers per 8 integers */
    idx_t q = (n - 1) / 5;            /* number of full 8‑integer blocks before a(n) */
    idx_t r = (n - 1) % 5;            /* position within the current block (0..4) */

    /* residues of accepted numbers in each block, in increasing order  */
    static const seq_t residues[5] = {2, 4, 5, 6, 7};

    return ((seq_t)q) * 8 + residues[r];
}
