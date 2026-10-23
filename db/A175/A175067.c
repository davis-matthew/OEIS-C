/*
 * A175067  a(n) is the sum of perfect divisors of n, where a perfect divisor
 *          of n is a divisor d such that d^k = n for some k >= 1.
 * https://oeis.org/A175067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum of perfect divisors of n, i.e., all d such that d^k =
 *            n for some k ≥ 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff base^exp ≤ limit (no overflow). */
static bool pow_leq(uint64_t base, uint32_t exp, uint64_t limit)
{
    __uint128_t prod = 1;
    for (uint32_t i = 0; i < exp; ++i) {
        prod *= base;
        if (prod > limit) return false;
    }
    return true;
}

/* Compute base^exp as __uint128_t; if it exceeds limit, return a value > limit. */
static __uint128_t pow_limit(uint64_t base, uint32_t exp, uint64_t limit)
{
    __uint128_t prod = 1;
    for (uint32_t i = 0; i < exp; ++i) {
        prod *= base;
        if (prod > limit) return (__uint128_t)limit + 1;
    }
    return prod;
}

/* Integer k‑th root: largest r such that r^k ≤ n. */
static uint64_t kth_root(uint64_t n, uint32_t k)
{
    if (k == 1) return n;
    uint64_t low = 2, high = n, ans = 1;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (pow_leq(mid, k, n)) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* safety */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)n;                /* always include n itself */
    if (n <= 1) return sum;

    uint64_t N = (uint64_t)n;
    for (uint32_t k = 2; ; ++k) {
        if (!pow_leq(2, k, N)) break;    /* no base ≥2 can satisfy r^k ≤ N */
        uint64_t r = kth_root(N, k);
        if (r < 2) continue;             /* skip trivial root 1 */
        if (pow_limit(r, k, N) == N)
            sum += (seq_t)r;
    }
    return sum;
}
