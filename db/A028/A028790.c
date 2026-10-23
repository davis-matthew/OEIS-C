/*
 * A028790  Nonsquares mod 77.
 * https://oeis.org/A028790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_residue_mod77
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-square residues modulo 77: return the nth positive integer that is not a
   quadratic residue modulo 77. */

#include <stdbool.h>
#include "../utils/is_residue_mod77.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    for (;;) {
        if (!is_residue_mod77(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate++;
    }
}
