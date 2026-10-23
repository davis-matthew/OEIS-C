/*
 * A046032  a(n) = (n!)^2 - 1.
 * https://oeis.org/A046032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^2 - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* n ≥ 1 (OEIS offset 1). */
    seq_t f = factorial(n);
    return f * f - 1;
}
