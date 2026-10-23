/*
 * A120882  a(n) is the number of k's, for 1 <= k <= n, where
 *          gcd(k,floor(n/k)) = 1.
 * https://oeis.org/A120882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | 1 ≤ k ≤ n, gcd(k, ⌊n/k⌋) = 1 }
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple Euclidean algorithm on uint64_t; result fits in seq_t
 * (the gcd is at most min(k,⌊n/k⌋) ≤ n). */
static inline seq_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0U) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0U;

    for (idx_t k = 1U; k <= n; ++k) {
        idx_t v = n / k;          /* floor(n/k) */
        if (gcd_uint64((uint64_t)k, (uint64_t)v) == 1U) {
            cnt = cnt + 1U;
        }
    }

    return cnt;
}
