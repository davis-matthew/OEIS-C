/*
 * A088963  Primes p such that 5p is the hypotenuse belonging to two
 *          prime-free primitive Pythagorean triples.
 * https://oeis.org/A088963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth prime p such that 5p is the hypotenuse of two
 * primitive Pythagorean triples, and every leg of both triples is composite.
 * The value is found by exhaustive search of primes p ≡ 1 (mod 4)
 * and checking the two primitive representations of 5p.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/gcd_u64.h"

/*----- small integer utilities -----*/

static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid) hi = mid - 1;
        else                lo = mid;
    }
    return lo;
}

static inline uint64_t isqrt_floor_u64(uint64_t n)
{
    return isqrt_u64(n);
}

static inline void ensure_cache(size_t need, seq_t **cache, size_t *cap)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(**cache));
    *cache = tmp;
    *cap   = new_cap;
}

/*----- find representation of prime p = x^2 + y^2 -----*/
static bool rep_prime(uint64_t p, uint64_t *x, uint64_t *y)
{
    uint64_t lim = isqrt_floor_u64(p);
    for (uint64_t v = 1; v <= lim; ++v) {
        uint64_t t = p - v * v;
        uint64_t u = isqrt_floor_u64(t);
        if (u * u == t && u > v && gcd_u64(u, v) == 1) {
            *x = u;
            *y = v;
            return true;
        }
    }
    return false;   /* should not happen for prime ≡ 1 (mod 4) */
}

/*----- test composite legs for the two triples -----*/
static bool legs_composite(uint64_t u, uint64_t v)
{
    /* primitive triple legs */
    seq_t a = (seq_t)u * (seq_t)u - (seq_t)v * (seq_t)v;
    seq_t b = (seq_t)2 * (seq_t)u * (seq_t)v;
    return !is_prime_big(a) && !is_prime_big(b);
}

/*----- main sequence function -----*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t curr_p = 5;   /* start just above the smallest term */

    while (cache_len < n) {
        /* find next prime p ≡ 1 (mod 4) */
        do {
            ++curr_p;
            if (!is_prime_big((seq_t)curr_p)) continue;
            if ((curr_p & 3) != 1) continue;
            break;   /* candidate p */
        } while (true);

        uint64_t p = curr_p;

        /* find representation p = x^2 + y^2 */
        uint64_t x, y;
        if (!rep_prime(p, &x, &y)) continue;    /* should not happen */

        /* 5 = 2^2 + 1^2 */
        const uint64_t a5 = 2, b5 = 1;

        /* Two primitive representations of 5p */
        /* first: (a5*x - b5*y, a5*y + b5*x) */
        uint64_t u1 = a5 * x - b5 * y;
        uint64_t v1 = a5 * y + b5 * x;
        if (u1 < v1) { uint64_t t = u1; u1 = v1; v1 = t; }

        /* second: (a5*x + b5*y, |a5*y - b5*x|) */
        uint64_t u2 = a5 * x + b5 * y;
        uint64_t v2 = (a5 * y >= b5 * x) ? (a5 * y - b5 * x)
                                          : (b5 * x - a5 * y);
        if (u2 < v2) { uint64_t t = u2; u2 = v2; v2 = t; }

        /* check both triples have only composite legs */
        if (!legs_composite(u1, v1) || !legs_composite(u2, v2))
            continue;    /* reject this p */

        /* accept p and store it */
        ensure_cache(cache_len + 1, &cache, &cache_cap);
        cache[cache_len++] = (seq_t)p;
    }

    return cache[n - 1];
}
