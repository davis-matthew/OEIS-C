/*
 * A001026  Powers of 17: a(n) = 17^n.
 * https://oeis.org/A001026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1), a(0) = 1 (i.e. a(n) = 17^n), via binary
 *            exponentiation
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   a001026_pow17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001026_pow17.h"

seq_t a(idx_t n) {
    return a001026_pow17(n);
}
