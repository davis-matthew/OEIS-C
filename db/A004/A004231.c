/*
 * A004231  Ackermann's sequence: n^^n := n^n^n^...^n (with n n's).
 * https://oeis.org/A004231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^^n (n tetrated to n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t tetrate(seq_t base, idx_t height) {
    if (height == 0) return 1;
    if (height == 1) return base;
    return a000248_pow(base, (idx_t)tetrate(base, height - 1));
}

seq_t a(idx_t n) {
    return tetrate((seq_t)n, n);
}
