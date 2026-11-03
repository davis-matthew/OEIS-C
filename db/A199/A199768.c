/*
 * A199768  Numbers whose greatest prime factor is less than their number of
 *          divisors.
 * https://oeis.org/A199768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer x such that the greatest prime
 *            factor of x is strictly smaller than the number of divisors of x
 * Range:     u8 n <= 59, u16 n <= 3423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8772 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_divisors.h"
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

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_test = 2;   /* next integer to test (starting at 2) */

static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();                /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the sequence */

    while (cache_len < (size_t)n) {
        uint64_t x = next_test++;
        uint64_t gpf = largest_prime_divisor(x);
        uint64_t d   = num_divisors(x);
        if (gpf < d) {
            grow_cache();
            cache[cache_len++] = (seq_t)x;
        }
    }
    return cache[n - 1];
}
