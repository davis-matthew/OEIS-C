/*
 * A028748  Nonsquares mod 35.
 * https://oeis.org/A028748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n‑th positive integer that is NOT a quadratic
 *            residue modulo 35
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Quadratic residues modulo 35: 0,1,4,9,11,14,15,16,21,25,29,30 */
static const bool is_residue[35] = {
    true,  /* 0  */
    true,  /* 1  */
    false, /* 2  */
    false, /* 3  */
    true,  /* 4  */
    false, /* 5  */
    false, /* 6  */
    false, /* 7  */
    false, /* 8  */
    true,  /* 9  */
    false, /*10  */
    true,  /*11  */
    false, /*12  */
    false, /*13  */
    true,  /*14  */
    true,  /*15  */
    true,  /*16  */
    false, /*17  */
    false, /*18  */
    false, /*19  */
    false, /*20  */
    true,  /*21  */
    false, /*22  */
    false, /*23  */
    false, /*24  */
    true,  /*25  */
    false, /*26  */
    false, /*27  */
    false, /*28  */
    true,  /*29  */
    true,  /*30  */
    false, /*31  */
    false, /*32  */
    false, /*33  */
    false  /*34  */
};

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 1; ; ++i) {
        if (!is_residue[(int)(i % 35)]) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}
