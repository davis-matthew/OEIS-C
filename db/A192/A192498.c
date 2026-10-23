/*
 * A192498  Smallest prime p such that there is a gap of tau(n) between p and
 *          the next prime, otherwise 0.
 * https://oeis.org/A192498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; if n is a perfect square a(n) = 0, otherwise a(n) =
 *            the smallest prime p such that the next prime after p is exactly
 *            p + τ(n), i.e. a(n) = A000230(τ(n)). τ(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 119, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, divisor_count_u64, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/divisor_count_u64.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

#define GAP_CACHE_SIZE 512

/* Cached result for each prime gap up to GAP_CACHE_SIZE.
   Index g holds the smallest prime p with prime gap = g.
   Zero means uncomputed (cache never really contains a valid prime 0). */
static seq_t gap_cache[GAP_CACHE_SIZE] = {0};

/* Smallest prime p with prime gap exactly `gap`.
   For gaps below the cache size cached results are reused. */
static seq_t smallest_prime_with_gap(uint64_t gap)
{
    if (gap < GAP_CACHE_SIZE && gap_cache[gap] != 0)
        return gap_cache[gap];

    for (uint64_t p = 2; ; ++p) {
        if (!is_prime_big((seq_t)p))
            continue;
        uint64_t next = next_prime(p);          /* next prime after p */
        if (next == p + gap) {
            seq_t result = (seq_t)p;
            if (gap < GAP_CACHE_SIZE)
                gap_cache[gap] = result;
            return result;
        }
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    /* If n is a perfect square, τ(n) is odd → no even prime gap → 0 */
    idx_t root = _floor_isqrt_u64(n);
    if (root * root == n)
        return (seq_t)0;

    /* τ(n) always fits in uint64_t for n up to 10⁴ */
    uint64_t tau = divisor_count_u64((uint64_t)n);

    /* τ(n) is even for non‑square n, so use it as the required gap */
    return smallest_prime_with_gap(tau);
}
