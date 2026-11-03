/*
 * A231813  Number of iterations of A046665(n) = (greatest prime divisor of n)
 *          - (least prime divisor of n) [with A046665(1) = 0] required to
 *          reach zero.
 * https://oeis.org/A231813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 8913 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) counts how many times you must apply
 * A046665(n) = (greatest prime divisor of n) - (least prime divisor of n)
 * until you reach 0.  Its recurrence is:
 *
 *  a(0) = 0
 *  a(n) = 1 + a(A046665(n))   for n > 0
 *
 *  Example:  a(6) = 1 + a(3−2) = 1 + a(1) = 1 + (1 + a(0)) = 2
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

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

/* Return a(n).  The function is memoised so that any order of calls
 * is efficient.  It works for all n that fit in idx_t (typically uint64_t). */
seq_t a(idx_t n)
{
    /* Static cache of computed values.  cache[0] is unused (a(0) = 0). */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;

    /* Ensure the cache can hold index n.  New entries are initialised to 0. */
    if (cache == NULL || n >= cache_sz) {
        size_t new_sz = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_sz);
        if (tmp == NULL)          /* allocation failure – give up gracefully */
            return 0;
        cache = tmp;
        /* initialise new slots to 0; existing data is preserved */
        for (size_t i = cache_sz; i < new_sz; ++i)
            cache[i] = 0;
        cache_sz = new_sz;
    }

    if (n == 0)          /* a(0) = 0 */
        return 0;

    if (cache[n] != 0)   /* already computed */
        return cache[n];

    /* Base case: a(1) = 1 */
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }

    /* Compute A046665(n) = greatest prime divisor – least prime divisor */
    uint64_t lpf = smallest_prime_factor((uint64_t)n);      /* least prime divisor */
    uint64_t gpf = largest_prime_divisor((uint64_t)n);      /* greatest prime divisor */
    idx_t    diff = (idx_t)(gpf - lpf);                      /* difference fits in idx_t */

    seq_t result = 1 + a(diff);               /* recursive definition */

    cache[n] = result;
    return result;
}
