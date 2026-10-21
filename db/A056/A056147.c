/*
 * A056147  a(1) = 1, a(m+1) = Sum_{k=1..m} lcm(m, a(k)).
 * https://oeis.org/A056147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(n+1) = Σ_{k=1..n} lcm(n, a(k)) (OEIS A056147)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* 1‑based cache of computed values. */
static seq_t *cache = NULL;   /* array of seq_t, indices 0..cache_cap, 0 unused */
static size_t cache_cap = 0;  /* current capacity (includes element 0) */
static idx_t  cache_last = 0; /* largest n that has been computed */

/* expand cache so that indices up to n are valid */
static void grow_cache(idx_t n)
{
    if (n <= cache_cap) return;

#if defined(__cplusplus)
    seq_t *tmp = new seq_t[n + 1];   /* default‑constructed seq_t elements */
    if (cache) {
        for (size_t i = 0; i <= cache_cap; ++i)
            tmp[i] = cache[i];
    }
    delete[] cache;
    cache = tmp;
    /* zero unused entry */
    cache[0] = 0;
#else
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp && (n + 1) != 0) abort();    /* out of memory */
    cache = tmp;
    /* initialise newly allocated elements to zero */
    for (size_t i = cache_cap + 1; i <= n; ++i)
        cache[i] = 0;
    /* zero unused entry */
    cache[0] = 0;
#endif

    cache_cap = n;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1; safe default */

    grow_cache(n);

    if (cache_last < n) {
        for (idx_t cur = cache_last + 1; cur <= n; ++cur) {
            if (cur == 1) {
                cache[1] = 1;
            } else {
                seq_t sum   = 0;
                seq_t m_val = (seq_t)(cur - 1);   /* m in the recurrence */

                for (idx_t k = 1; k <= cur - 1; ++k) {
                    seq_t g   = gcd(m_val, cache[k]);   /* gcd(m, a_k) */
                    seq_t l   = (m_val / g) * cache[k]; /* lcm(m, a_k) */
                    sum += l;
                }
                cache[cur] = sum;
            }
        }
        cache_last = n;
    }

    return cache[n];
}
