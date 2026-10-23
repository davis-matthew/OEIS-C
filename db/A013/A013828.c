/*
 * A013828  a(n) = 3^(5*n + 3).
 * https://oeis.org/A013828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(5*n + 3) Recurrence: a(0) = 27, a(n) = 243 * a(n-1)
 *            for n > 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = (seq_t)27;  // a(0)
    for (idx_t i = 0; i < n; ++i) {
        result *= (seq_t)243;  // multiply by 3^5 = 243
    }
    return result;
}
