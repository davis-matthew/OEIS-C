/*
 * A001280  Number of permutations of length n by rises.
 * https://oeis.org/A001280
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n-8} binomial(m+4,4) * binomial(n-m-5,3) *
 *            D(m+4), where D(p) = A000255(p-1) counts permutations of [p]
 *            with no i immediately followed by i+1: D(p) = (p-1)*D(p-1) +
 *            (p-2)*D(p-2), D(1) = 1, D(2) = 1. (a(n) = A136123(n,4):
 *            permutations with exactly 4 maximal runs i,i+1,...,i+j of l...
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   a001280_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001280_bin.h"

seq_t a(idx_t n) {
    seq_t dp1, dp, sum, next;
    idx_t m;
    if (n < 8) return 0;
    dp1 = 3;   /* D(3) */
    dp = 11;   /* D(4) */
    sum = 0;
    for (m = 0; m + 8 <= n; m++) {
        idx_t p = m + 4;
        sum += a001280_bin(m + 4, 4) * a001280_bin(n - m - 5, 3) * dp;
        if (m + 8 == n) break;
        next = (seq_t)p * dp + (seq_t)(p - 1) * dp1;
        dp1 = dp;
        dp = next;
    }
    return sum;
}
