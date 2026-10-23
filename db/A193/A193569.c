/*
 * A193569  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 89.
 * https://oeis.org/A193569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer that is unhappy and first reaches
 *            the cycle (4,16,37,58,89,145,42,20) at 89
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   hit_89_first, sumsqdigits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/hit_89_first.h"
#include "../utils/sumsqdigits.h"

/* Return the nth element of the sequence. */
seq_t a(idx_t n) {
    idx_t pos = 0;          /* count of qualifying numbers found so far */

    for (seq_t x = 1; ; ++x) {
        if (hit_89_first(x)) {
            ++pos;
            if (pos == n)
                return x;
        }
    }
}
