/*
 * A002021  Pile of coconuts problem: (n-1)*(n^n - 1), n even; n^n - n + 1, n
 *          odd.
 * https://oeis.org/A002021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (n-1)*(n^n - 1) for n even; a(n) = n^n - n + 1 for n odd
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   a002021_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002021_pow.h"

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t nn = a002021_pow(ns, n);
    if (n % 2 == 0) {
        return (ns - 1) * (nn - 1);
    } else {
        return nn - ns + 1;
    }
}
