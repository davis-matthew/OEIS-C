/*
 * A044614  Numbers n such that string 5,6 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   oct_has56
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth integer that contains the octal substring "56" but whose successor does not. */
#include <stdbool.h>
#include "../utils/oct_has56.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 1, guard only */
    idx_t found = 0;
    for (idx_t i = 0; ; ++i)
    {
        seq_t v = (seq_t)i;
        if (oct_has56(v) && !oct_has56(v + (seq_t)1))
        {
            ++found;
            if (found == n)
                return v;
        }
    }
}
