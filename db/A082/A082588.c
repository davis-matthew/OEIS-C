/*
 * A082588  a(1) = 1, a(n) = Sum_{d | n and d < n} a(d)^2 for n > 1.
 * https://oeis.org/A082588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d<n} a(d)^2 , a(1) = 1 The sequence is computed
 *            by memoizing previously computed values and enumerating divisors
 *            up to sqrt(n) for each n
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 95, u64 n <= 191,
 *            u128 n <= 383, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoised values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that the cache can address indices up to 'n' */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* fatal if out of memory */
    /* Zero new entries */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Recursively compute a(n) with memoisation and divisor enumeration */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0 */
    ensure_cache(n);
    if (n == 1) { cache[1] = 1; return 1; }
    if (cache[n] != 0) return cache[n];

    seq_t sum = 0;
    idx_t d = 1;
    for (; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (d != n) {
                if (cache[d] == 0)
                    cache[d] = a(d);
                sum += cache[d] * cache[d];
            }
            idx_t q = n / d;              /* complementary divisor */
            if (q != d && q != n) {
                if (cache[q] == 0)
                    cache[q] = a(q);
                sum += cache[q] * cache[q];
            }
        }
    }

    cache[n] = sum;
    return sum;
}
