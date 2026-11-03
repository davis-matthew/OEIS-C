/*
 * A014840  Sum of all the digits of n in every base prime to n from 2 to n-1.
 * https://oeis.org/A014840
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∑_{b=2}^{n–1, gcd(b,n)=1} digit_sum(n,b)
 * Range:     u8 n <= 36, u16 n <= 598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9633 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"

/* Sum of digits of n expressed in base b */
static inline seq_t _digit_sum_in_base(idx_t n, idx_t b)
{
    seq_t s = 0;
    while (n > 0) {
        s += (seq_t)(n % b);
        n /= b;
    }
    return s;
}

/* Euclidean algorithm for idx_t */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t total = 0;
    for (idx_t b = 2; b < n; ++b) {
        if (gcd_idx(n, b) != 1) continue;
        total += _digit_sum_in_base(n, b);
    }
    return total;
}
