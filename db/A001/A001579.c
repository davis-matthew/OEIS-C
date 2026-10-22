/*
 * A001579  a(n) = 3^n + 5^n + 6^n.
 * https://oeis.org/A001579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 5^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"


seq_t a(idx_t n) {
    return a000248_pow((seq_t)3, n) + a000248_pow((seq_t)5, n) + a000248_pow((seq_t)6, n);
}
