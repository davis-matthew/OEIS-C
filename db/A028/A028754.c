/*
 * A028754  Nonsquares mod 41.
 * https://oeis.org/A028754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th positive integer that is NOT a quadratic residue
 *            modulo 41
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Residue table for squares modulo 41 (index 0..40). */
    static const bool is_res[41] = {
        true,  /* 0  */
        true,  /* 1  */
        true,  /* 2  */
        false, /* 3  */
        true,  /* 4  */
        true,  /* 5  */
        false, /* 6  */
        false, /* 7  */
        true,  /* 8  */
        true,  /* 9  */
        true,  /*10  */
        false, /*11  */
        false, /*12  */
        false, /*13  */
        false, /*14  */
        false, /*15  */
        true,  /*16  */
        false, /*17  */
        true,  /*18  */
        false, /*19  */
        true,  /*20  */
        true,  /*21  */
        false, /*22  */
        true,  /*23  */
        false, /*24  */
        true,  /*25  */
        false, /*26  */
        false, /*27  */
        false, /*28  */
        false, /*29  */
        false, /*30  */
        true,  /*31  */
        true,  /*32  */
        true,  /*33  */
        false, /*34  */
        false, /*35  */
        true,  /*36  */
        true,  /*37  */
        false, /*38  */
        true,  /*39  */
        true   /*40  */
    };

    idx_t count = 0;
    idx_t num   = 1;

    while (1) {
        if (!is_res[num % 41]) {
            count++;
            if (count == n)
                return (seq_t)num;
        }
        num++;
    }
}
