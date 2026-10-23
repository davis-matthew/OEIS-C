/*
 * A028726  Nonsquares mod 13.
 * https://oeis.org/A028726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of positive integers that are not quadratic residues modulo 13. */
/* a(n) = 13 * ⌊(n−1)/6⌋ + r[(n−1) mod 6],  where r = {2,5,6,7,8,11}. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Indexing starts at 1 */
    idx_t q = (n - 1) / 6;            /* quotient */
    idx_t r = (n - 1) % 6;            /* remainder index */

    /* Residues that are not quadratic residues modulo 13 */
    static const unsigned char residues[6] = {2,5,6,7,8,11};

    return ((seq_t)q) * 13 + (seq_t)residues[r];
}
