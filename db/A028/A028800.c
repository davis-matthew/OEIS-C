/*
 * A028800  Nonsquares mod 87.
 * https://oeis.org/A028800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth positive integer that is NOT a quadratic residue mod
 *            87
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
/* Quadratic residues modulo 29 */
static const bool sq29[29] = {
    true,  /* 0  */
    true,  /* 1  */
    false, /* 2  */
    false, /* 3  */
    true,  /* 4  */
    true,  /* 5  */
    true,  /* 6  */
    true,  /* 7  */
    false, /* 8  */
    true,  /* 9  */
    false, /*10  */
    false, /*11  */
    false, /*12  */
    true,  /*13  */
    false, /*14  */
    false, /*15  */
    true,  /*16  */
    false, /*17  */
    false, /*18  */
    false, /*19  */
    true,  /*20  */
    false, /*21  */
    true,  /*22  */
    true,  /*23  */
    true,  /*24  */
    true,  /*25  */
    false, /*26  */
    false, /*27  */
    true   /*28  */
};

seq_t a(idx_t n)
{
    /* Residues modulo 87 that are quadratic residues. */
    static bool resid[87];
    static bool init = false;

    if (!init) {
        for (unsigned i = 0; i < 87; ++i) {
            /* i is a square mod 3 iff i % 3 == 0 or 1. */
            bool sq3 = (i % 3 != 2);
            bool sq29_res = sq29[i % 29];
            resid[i] = sq3 && sq29_res;
        }
        init = true;
    }

    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        if (!resid[x % 87]) {
            ++count;
            if (count == n) {
                return (seq_t)x;
            }
        }
    }
}
