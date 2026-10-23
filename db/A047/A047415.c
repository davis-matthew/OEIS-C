/*
 * A047415  Numbers that are congruent to {1, 3, 4, 6} mod 8.
 * https://oeis.org/A047415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {1,3,4,6}[(n-1)%4] (n >= 1)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The four residues modulo 8 that belong to the sequence */
    static const unsigned char residues[4] = {1, 3, 4, 6};

    /* Convert the index into the zero‑based position */
    idx_t zero = n - 1U;
    /* Determine which block of four numbers we are in */
    idx_t q = zero / 4U;
    /* Determine the position inside that block */
    idx_t r = zero % 4U;

    /* Compute the term using only integer arithmetic */
    return (seq_t)((seq_t)q * 8U + (seq_t)residues[r]);
}
