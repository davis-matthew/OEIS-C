/*
 * A028747  Nonsquares mod 34.
 * https://oeis.org/A028747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_square_mod_34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028747: a(n) = the n‑th positive integer that is not a quadratic
   residue modulo 34. */
#include <stdbool.h>
#include "../utils/is_square_mod_34.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val   = 0;

    /* Find the n‑th positive integer that is not a quadratic residue mod 34. */
    while (count < n)
    {
        ++val;                        /* start from 1,2,3,... */
        if (!is_square_mod_34(val))
            ++count;
    }
    return val;
}
