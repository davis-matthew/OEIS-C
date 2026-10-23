/*
 * A044671  Numbers n such that string 4,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑9
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_substring_43
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// representation contains the substring "43" while the
// representation of n+1 does NOT contain "43".

#include <stdbool.h>
#include "../utils/has_substring_43.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but guard against 0 */

    idx_t found = 0;          /* number of terms found so far */
    seq_t num = (seq_t)0;     /* start from 1 */

    for (;;) {
        ++num;                         /* candidate number */

        if (has_substring_43(num) && !has_substring_43(num + (seq_t)1)) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
