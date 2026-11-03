/*
 * A137809  a(0) = a(1) = 1. a(n) = a(n-1) + a(n - b(n)), where b(n) is
 *          largest prime dividing n.
 * https://oeis.org/A137809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 a(n) = a(n-1) + a(n - b(n)) (for n ≥ 2) where
 *            b(n) is the largest prime divisor of n
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 107, u64 n <= 269,
 *            u128 n <= 685, big any n
 * Validated: standard, pass, 64 terms through n = 9624 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    static seq_t *cache = NULL;          /* memoised sequence values */
    static idx_t cache_len = 0;          /* allocated size of cache */
    static idx_t last = 0;               /* highest index that has been computed */

    /* trivial cases */
    if (n < 2)
        return (seq_t)1;

    /* ensure cache is large enough */
    if (cache_len < n + 1) {
        idx_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_len);
        if (!tmp) {
            /* out of memory; caller must handle */
            abort();
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* initialise first two values if they have not been set yet */
    if (last < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last = 1;
    }

    /* compute missing entries up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        uint64_t lp = largest_prime_divisor((uint64_t)i);
        idx_t j = i - (idx_t)lp;
        cache[i] = cache[i - 1] + cache[j];
    }

    last = n;
    return cache[n];
}
