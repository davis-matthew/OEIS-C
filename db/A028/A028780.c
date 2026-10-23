/*
 * A028780  Nonsquares mod 67.
 * https://oeis.org/A028780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Non-squares modulo 67.
    a(n) = the n-th positive integer that is NOT a quadratic residue modulo 67. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute the set of quadratic residues modulo 67.  */
    static unsigned char residues[67] = {0};
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        for (idx_t i = 0; i < 67; ++i) {
            idx_t r = (i * i) % 67;
            residues[r] = 1;          /* mark residue */
        }
    }

    idx_t count = 0;
    idx_t candidate = 1;

    while (true) {
        if (!residues[candidate % 67]) {   /* candidate is a non-residue */
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
