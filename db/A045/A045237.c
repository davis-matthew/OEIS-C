/*
 * A045237  Numbers whose base-5 representation contains exactly one 1 and one
 *          3.
 * https://oeis.org/A045237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation
 *            contains exactly one digit 1 and exactly one digit 3. The
 *            function searches forward from 0 until it has found n such
 *            numbers
 * Range:     u8 n <= 45, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_one_and_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_one_and_three.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we simply
       return 0 – the behaviour for indices below the offset does not
       matter according to the specification. */
    if (n == 0)
        return 0;

    idx_t found = 0;
    seq_t val = 0;

    while (1) {
        if (has_one_and_three(val)) {
            if (++found == n)
                return val;
        }
        ++val;
    }
}
