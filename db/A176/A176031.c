/*
 * A176031  a(n) = n^rad(n).
 * https://oeis.org/A176031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^rad(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   rad, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* rad(n) is the product of distinct prime factors of n */
    seq_t r = rad(n);
    /* exponent rad(n) fits in idx_t because rad(n) <= n */
    return seq_pow((seq_t)n, (idx_t)r);
}
