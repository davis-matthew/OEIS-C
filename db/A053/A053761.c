/*
 * A053761  Least positive integer k for which the Jacobi symbol (k|2*n-1) is
 *          less than 1, where 2*n-1 is a nonsquare; a(n)=0 if 2*n-1 is a
 *          square.
 * https://oeis.org/A053761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if 2*n-1 is a perfect square; otherwise the least k>0
 *            such that the Jacobi symbol (k|2*n-1) is < 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9991 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* not defined for 0, guard */
    uint64_t n64 = (uint64_t)n;
    uint64_t m = n64 * 2 - 1;                /* 2*n-1 */
    uint64_t sq = isqrt_u64(m);
    if (sq * sq == m)
        return (seq_t)0;                      /* m is a perfect square */
    for (uint64_t k = 2; ; ++k) {
        if (jacobi_symbol(k, m) < 1)
            return (seq_t)k;
    }
}
