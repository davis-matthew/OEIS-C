/*
 * A028761  Nonsquares mod 48.
 * https://oeis.org/A028761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028761: Nonsquares mod 48
 * a(n) is the n‑th positive integer that is NOT a quadratic residue modulo 48.
 * The quadratic residues modulo 48 are {0,1,4,9,16,25,33,36}.
 * The function scans the positive integers, skipping any value whose
 * remainder modulo 48 is one of the above residues.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remainders that are quadratic residues modulo 48 (1=residue, 0=non‑residue). */
    static const uint8_t is_residue[48] = {
        /* 0 */ 1,
        /* 1 */ 1,
        /* 2 */ 0,
        /* 3 */ 0,
        /* 4 */ 1,
        /* 5 */ 0,
        /* 6 */ 0,
        /* 7 */ 0,
        /* 8 */ 0,
        /* 9 */ 1,
        /*10 */ 0,
        /*11 */ 0,
        /*12 */ 0,
        /*13 */ 0,
        /*14 */ 0,
        /*15 */ 0,
        /*16 */ 1,
        /*17 */ 0,
        /*18 */ 0,
        /*19 */ 0,
        /*20 */ 0,
        /*21 */ 0,
        /*22 */ 0,
        /*23 */ 0,
        /*24 */ 0,
        /*25 */ 1,
        /*26 */ 0,
        /*27 */ 0,
        /*28 */ 0,
        /*29 */ 0,
        /*30 */ 0,
        /*31 */ 0,
        /*32 */ 0,
        /*33 */ 1,
        /*34 */ 0,
        /*35 */ 0,
        /*36 */ 1,
        /*37 */ 0,
        /*38 */ 0,
        /*39 */ 0,
        /*40 */ 0,
        /*41 */ 0,
        /*42 */ 0,
        /*43 */ 0,
        /*44 */ 0,
        /*45 */ 0,
        /*46 */ 0,
        /*47 */ 0
    };

    idx_t  count = 0;  /* how many nonresidues have been seen */
    seq_t  x     = 0;  /* current integer, starting from 0 */

    for (;;)
    {
        x += (seq_t)1;              /* next integer */

        uint64_t r = (uint64_t)(x % (seq_t)48);  /* remainder modulo 48 */

        if (!is_residue[r])         /* non‑residue: contributes to the sequence */
        {
            ++count;
            if (count == n)
                return x;
        }
    }
}
