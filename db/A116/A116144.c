/*
 * A116144  a(n) = 4^n * n*(n+1).
 * https://oeis.org/A116144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * n * (n + 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t term = (seq_t)n * ((seq_t)n + 1);
    seq_t power = pow2(2 * n);
    return term * power;
}
