/*
 * A011595  Legendre symbol (n,61).
 * https://oeis.org/A011595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,61) = n^30 (mod 61) represented as 0,
 *            1, or -1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   legendre_mod61
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/legendre_mod61.h"

seq_t a(idx_t n) {
    return legendre_mod61(n);
}
