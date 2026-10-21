/*
 * A076973  Starting with 2, largest prime divisor of the sum of all previous
 *          terms.
 * https://oeis.org/A076973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(n)=largest prime divisor of sum_{k=1}^{n-1} a(k) (n≥2)
 * Range:     u8 n <= 506, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

/* static cache state */
static seq_t *cache   = NULL;   /* terms, 1‑based in cache[0].. */
static size_t cap     = 0;      /* current capacity of cache */
static idx_t  upto    = 0;      /* largest index already computed */
static uint64_t sum   = 0;      /* running sum of computed terms */

/* largest prime divisor of a 64‑bit unsigned integer */
static uint64_t _lpf_uint64(uint64_t n)
{
    uint64_t largest = 1;
    /* remove factor 2 */
    while ((n & 1ULL) == 0) {
        largest = 2;
        n >>= 1;
    }
    /* trial division by odd numbers */
    for (uint64_t i = 3; i <= n / i; i += 2) {
        while (n % i == 0) {
            largest = i;
            n /= i;
        }
    }
    if (n > 1) largest = n;   /* remaining prime factor */
    return largest;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range – not used by OEIS */

    /* Ensure cache has space for n terms. */
    ensure_cache(n, &cache, &cap);

    /* If we already know the term, return it. */
    if ((idx_t)n <= upto) return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = upto + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)2;            /* initial term */
        } else {
            val = (seq_t)_lpf_uint64(sum);
        }
        cache[i - 1] = val;
        sum += (uint64_t)val;          /* update running sum */
    }

    upto = n;
    return cache[n - 1];
}
