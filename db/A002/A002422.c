/*
 * A002422  Expansion of (1-4*x)^(5/2).
 * https://oeis.org/A002422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*(2*n-7)*a(n-1)/n, a(0) = 1 (D-finite recurrence n*a(n)
 *            + 2*(-2*n+7)*a(n-1) = 0)
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 23, i64 n <= 40, i128 n <= 74,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 950 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t r = result % (seq_t)i;
        long long rl = (long long)r;
        if (rl < 0) rl = -rl;
        uint64_t g = gcd_u64((uint64_t)i, (uint64_t)rl);
        /* result/g and i/g are coprime, so i/g divides 2*(2i-7) exactly */
        seq_t q = (seq_t)(i / g);
        seq_t t = 2 * (2 * (seq_t)i - 7) / q;
        result = (result / (seq_t)g) * t;
    }
    return result;
}
