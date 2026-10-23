/*
 * A028770  Nonsquares mod 57.
 * https://oeis.org/A028770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028770: Nonsquares mod 57
   a(n) = n-th positive integer that is not a quadratic residue modulo 57.
*/

#include <stdbool.h>

/* Sequence type and index type are supplied by the build. */
seq_t a(idx_t n)
{
    /* The set of quadratic residues modulo 57 (0..56). */
    static const bool is_residue[57] = {
        /* 0 */ true,
        /* 1 */ true,
        /* 2 */ false,
        /* 3 */ false,
        /* 4 */ true,
        /* 5 */ false,
        /* 6 */ true,
        /* 7 */ true,
        /* 8 */ false,
        /* 9 */ true,
        /*10 */ false,
        /*11 */ false,
        /*12 */ false,
        /*13 */ false,
        /*14 */ false,
        /*15 */ false,
        /*16 */ true,
        /*17 */ false,
        /*18 */ false,
        /*19 */ true,
        /*20 */ false,
        /*21 */ false,
        /*22 */ false,
        /*23 */ false,
        /*24 */ true,
        /*25 */ true,
        /*26 */ false,
        /*27 */ false,
        /*28 */ true,
        /*29 */ false,
        /*30 */ true,
        /*31 */ false,
        /*32 */ false,
        /*33 */ false,
        /*34 */ false,
        /*35 */ false,
        /*36 */ true,
        /*37 */ false,
        /*38 */ false,
        /*39 */ true,
        /*40 */ false,
        /*41 */ false,
        /*42 */ true,
        /*43 */ true,
        /*44 */ false,
        /*45 */ true,
        /*46 */ false,
        /*47 */ false,
        /*48 */ false,
        /*49 */ true,
        /*50 */ false,
        /*51 */ false,
        /*52 */ false,
        /*53 */ false,
        /*54 */ true,
        /*55 */ true,
        /*56 */ false
    };

    if (n == 0) return 0;          /* offset is 1, but handle zero gracefully */
    idx_t cnt = 0;                  /* number of non–residues seen so far */
    uint64_t cur = 1;               /* candidate value */

    for (;; ++cur) {
        unsigned r = (unsigned)(cur % 57);   /* remainder modulo 57 */
        if (!is_residue[r]) {                /* non–quadratic residue */
            ++cnt;
            if (cnt == n)
                return (seq_t)cur;           /* cast to seq_t before returning */
        }
    }
}
