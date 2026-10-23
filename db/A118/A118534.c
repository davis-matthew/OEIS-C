/*
 * A118534  a(n) is the largest k such that prime(n+1) = prime(n) + (prime(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A118534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118534: a(n) = largest k such that prime(n+1)=prime(n)+(prime(n) mod k);
   0 if no such k exists.  The sequence uses 1‑based indexing. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* dynamic array of primes */
static uint64_t *primes = NULL;
static size_t     primes_cnt = 0;
static size_t     primes_cap = 0;
static uint64_t   next_prime_candidate = 2;

/* integer square‑root (floor) for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t r = 0, bit = 1ULL << 62;          /* largest power of four <= v */
    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* grow primes array until it contains at least (needed_index+1) primes */
static void ensure_primes(size_t needed_index)
{
    while (primes_cnt <= needed_index) {
        /* find next prime */
        while (!is_prime_small(next_prime_candidate))
            ++next_prime_candidate;

        /* resize array if necessary */
        if (primes_cnt == primes_cap) {
            size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            primes = tmp;
            primes_cap = new_cap;
        }

        primes[primes_cnt++] = next_prime_candidate;
        ++next_prime_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n=0, return 0 */

    /* we need primes[0..n] for prime(n) and prime(n+1) */
    ensure_primes((size_t)n);

    uint64_t p = primes[(size_t)n - 1];   /* prime(n)   */
    uint64_t q = primes[(size_t)n];       /* prime(n+1) */
    uint64_t d = q - p;                   /* gap between primes */
    uint64_t g = p - d;                   /* p - (p mod k) = p - d */

    uint64_t best = 0;                    /* best divisor of g > d */
    uint64_t sq = isqrt_u64(g);
    for (uint64_t i = 1; i <= sq; ++i) {
        if (g % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = g / i;
            if (d1 > d && d1 > best) best = d1;
            if (d2 > d && d2 > best) best = d2;
        }
    }
    return (seq_t)best;   /* zero if none found */
}
