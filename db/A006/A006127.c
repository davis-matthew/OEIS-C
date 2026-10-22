/*
 * A006127  a(n) = 2^n + n.
 * https://oeis.org/A006127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 100,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    return pw2(n) + (seq_t)n;
}
