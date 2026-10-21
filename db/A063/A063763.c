/*
 * A063763  Composite integers k whose largest prime factor > sqrt(k).
 * https://oeis.org/A063763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes composite integers k whose largest prime factor > sqrt(k) (OEIS A063763) */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor_u64.h"

/* Integer square root (floor) for unsigned 64‑bit integers */
static uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, ans = 0;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0)
        {
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid > n / mid)       /* mid*mid > n */
            hi = mid - 1;
        else
        {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* Sequence function: a(n) = nth composite integer k with largest prime factor > sqrt(k) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return zero */

    idx_t found = 0;
    uint64_t k = 4;               /* first composite integer */

    while (found < n)
    {
        k++;   /* investigate next integer */
        uint64_t lpf = largest_prime_factor_u64(k);
        if (lpf == k) continue;   /* prime, not composite */

        uint64_t sqrt_k = int_sqrt_u64(k);
        if (lpf > sqrt_k)
        {
            found++;
            if (found == n) return (seq_t)k;
        }
    }
    return (seq_t)0; /* unreachable, but keeps the compiler happy */
}
