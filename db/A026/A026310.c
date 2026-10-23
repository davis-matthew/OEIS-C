/*
 * A026310  sin(n) > cos(n+1).
 * https://oeis.org/A026310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that sin(k) > cos(k+1) in radians
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (sin((double)k) > cos((double)(k + 1))) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
