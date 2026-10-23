/*
 * A028784  Nonsquares mod 71.
 * https://oeis.org/A028784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonsquare residues modulo 71
   a(n) is the nth positive integer that is not a quadratic residue modulo 71
   (i.e., not congruent to k*k (mod 71) for any integer k).  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool residues[71] = {0};
    static bool initialized = 0;

    if (!initialized) {
        /* Pre‑compute the 35 quadratic residues mod 71.  For a prime p,
           exactly (p-1)/2 distinct residues exist; 71 is odd.  */
        for (idx_t k = 1; k <= 35; ++k) {
            idx_t r = (k * k) % 71;
            residues[r] = 1;
        }
        initialized = 1;
    }

    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        idx_t r = x % 71;
        if (r == 0)                /* skip multiples of 71 */
            continue;
        if (!residues[r]) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
