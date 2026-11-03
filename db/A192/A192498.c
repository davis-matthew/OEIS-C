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
 * Validated: standard, pass, 93 terms through n = 9082 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, next_prime, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/isqrt.h"
#include "../utils/next_prime.h"
#include "../utils/num_divisors.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    idx_t root = isqrt(n);
    if (root * root == n)
        return (seq_t)0;

    /* τ(n) always fits in uint64_t for n up to 10⁴ */
    uint64_t tau = num_divisors((uint64_t)n);

    /* τ(n) is even for non‑square n, so use it as the required gap */
    return smallest_prime_with_gap(tau);
}
