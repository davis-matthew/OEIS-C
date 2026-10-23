/*
 * A028781  Nonsquares mod 68.
 * https://oeis.org/A028781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028781: Nonsquares mod 68 – a(n) is the nth positive integer that is not a
   quadratic residue modulo 68.  The set of nonresidues repeats every 68
   integers, so we pre‑compute one period and combine it with n. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static data: table of the 50 numbers in 1..68 that are not squares modulo 68 */
    static seq_t offsets[50];
    static bool initialized = false;

    if (!initialized) {
        /* Quadratic residues modulo 17: 0,1,2,4,8,9,13,15,16 */
        static const bool resid_17[17] = {
            true,  /* 0  */
            true,  /* 1  */
            true,  /* 2  */
            false, /* 3  */
            true,  /* 4  */
            false, /* 5  */
            false, /* 6  */
            false, /* 7  */
            true,  /* 8  */
            true,  /* 9  */
            false, /*10  */
            false, /*11  */
            false, /*12  */
            true,  /*13  */
            false, /*14  */
            true,  /*15  */
            true   /*16  */
        };

        idx_t cnt = 0;
        for (idx_t i = 1; i <= 68; ++i) {
            bool is_square = ((i & 3) <= 1) && resid_17[i % 17];
            if (!is_square) {
                offsets[cnt++] = (seq_t)i;
            }
        }
        /* cnt should be 50; the table is now fully initialized */
        initialized = true;
    }

    idx_t period = (n - 1) / 50;           /* which full block of 68 numbers */
    idx_t inner  = (n - 1) % 50;           /* index inside the block       */
    seq_t value  = (seq_t)period * (seq_t)68 + offsets[inner];
    return value;
}
