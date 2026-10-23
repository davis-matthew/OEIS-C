/*
 * A119332  Expansion of (1+x)/(1-2x^4).
 * https://oeis.org/A119332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(floor(n/4)) if n mod 4 is 0 or 1, otherwise 0
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    // Check whether n mod 4 is 0 or 1
    if ((n % 4) <= 1) {
        return pow2(n / 4);
    } else {
        return (seq_t)0;
    }
}
