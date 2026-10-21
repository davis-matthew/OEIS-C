/*
 * A097974  Sum of distinct prime divisors of n which are <= sqrt(n).
 * https://oeis.org/A097974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of n that are <= sqrt(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Integer square root for 64‑bit unsigned integers.
 * Returns floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* terms for n = 1 or lower are 0 */

    uint64_t primes[64];       /* enough space for prime factors of n <= 2^64-1 */
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    idx_t sqrt_n = isqrt_u64(n);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((idx_t)primes[i] <= sqrt_n)
            sum += (seq_t)primes[i];
    }
    return sum;
}
