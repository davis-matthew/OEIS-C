/*
 * A161003  A list of the composite numbers divided by their largest prime
 *          factors.
 * https://oeis.org/A161003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n)/A052369(n) (composite number divided by its
 *            largest prime factor)
 * Range:     u8 n <= 412, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9560 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;            /* number of terms cached */
    static uint64_t next_candidate = 2;      /* next integer to test for composite */

    if (n == 0) return 0;                   /* offset 1, harmless */

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n) return cache[n-1];

    /* Ensure enough terms are cached */
    size_t needed = (size_t)n;
    while (cache_len < needed) {
        /* Find next composite number */
        while (next_candidate <= 1 || is_prime((idx_t)next_candidate)) {
            ++next_candidate;
        }
        uint64_t comp = next_candidate;
        ++next_candidate;

        /* Compute composite / largest prime factor */
        uint64_t lp   = largest_prime_divisor(comp);
        seq_t val = (seq_t)comp / (seq_t)lp;

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (tmp == NULL) {
                /* Allocation failed – abort gracefully */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
    }

    return cache[n-1];
}
