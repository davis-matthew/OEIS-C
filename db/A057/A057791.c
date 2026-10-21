/*
 * A057791  Sum[k^(n-k)], where sum is over positive integers, k, where k <= n
 *          and gcd(k,n) = 1.
 * https://oeis.org/A057791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{1≤k≤n, gcd(k,n)=1} k^{n−k}
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 28, u128 n <= 47,
 *            big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_gcd_idx.h"

/* Euclidean algorithm for idx_t. */

/* Fast exponentiation on seq_t (may overflow for 64‑bit builds, but
   values that fit are computed exactly). */
static seq_t _pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (_gcd_idx(k, n) == 1) {
            idx_t exp = n - k;
            sum += _pow_seq_t((seq_t)k, exp);
        }
    }
    return sum;
}
