/*
 * A001098  Multiply previous term by 2 and write in binary.
 * https://oeis.org/A001098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) = the decimal digits of the binary expansion of
 *            2*a(n-1), where a(n-1) is read as an ordinary decimal number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   a001098_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001098_step.h"

seq_t a(idx_t n) {
    seq_t x = 1;
    for (idx_t i = 2; i <= n; i++) x = a001098_step(x);
    return x;
}
