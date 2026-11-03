/*
 * A163399  a(n) = (the binary order of n)^sigma(n).
 * https://oeis.org/A163399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(log2(n))^sigma(n)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 311 (2026-11-01)
 * Helpers:   ilog2, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
#include "../utils/ipow.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t base;
    if (n == 1) {
        base = (seq_t)0;
    } else {
        /* ceil(log2(n)) == floor_log2_uint64(n-1) + 1 */
        base = (seq_t)(ilog2(n - 1) + 1);
    }
    seq_t sigma = sigma_of(n);
    return ipow(base, sigma);
}
