/*
 * A039028  Numbers whose base-6 representation has the same number of 3's and
 *          5's.
 * https://oeis.org/A039028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number whose base‑6 representation has the same count of
 *            digits 3 and 5. This is a brute‑force generation, scanning
 *            natural numbers and picking those that satisfy the digit‑balance
 *            condition
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _has_equal_3_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_has_equal_3_5.h"

seq_t a(idx_t n)
{
    seq_t v = 0;
    idx_t cnt = 0;
    for (;;) {
        if (_has_equal_3_5(v)) {
            ++cnt;
            if (cnt == n)
                return v;
        }
        ++v;
    }
}
