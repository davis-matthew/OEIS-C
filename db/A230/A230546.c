/*
 * A230546  Least positive integer k <= n such that 2*k^2-1 is a prime and n -
 *          k is a square, or 0 if such an integer k does not exist.
 * https://oeis.org/A230546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_small.h"

/* Computes OEIS A230546: the least k ≤ n such that 2*k²−1 is a prime and n−k is a square,
   returning 0 if no such k exists. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* safety for out‑of‑range indices */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t diff = n - k;
        idx_t s    = _floor_isqrt_u64(diff);
        if ((s * s) != diff)          /* n−k is not a perfect square */
            continue;

        uint64_t kk  = (uint64_t)k;
        uint64_t val = 2ULL * kk * kk - 1ULL;   /* 2*k²−1 */

        if (is_prime_small(val))
            return (seq_t)k;        /* first (least) k satisfying the conditions */
    }
    return 0;   /* no suitable k found */
}
