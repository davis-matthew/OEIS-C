/*
 * A097559  a(n) = number of terms among {a(1), a(2), a(3), ..., a(n-1)} that
 *          are coprime to n; a(1)=2.
 * https://oeis.org/A097559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ i | 1 ≤ i < n and gcd(a(i), n) = 1 }| , a(1) = 2
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd64.h"

/* Helper: Euclidean gcd for uint64_t */

/* Cache of computed terms (1‑based indexing).  */
static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_cap = 0;          /* allocated capacity (>= current size) */
static idx_t cache_cnt = 0;          /* number of computed terms (>=1) */

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; undefined for n==0 */

    /* First call: allocate cache and set a(1)=2. */
    if (cache == NULL) {
        cache_cap = 64;                /* initial capacity */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        cache[1] = (seq_t)2;           /* a(1) = 2 */
        cache_cnt = 1;
    }

    /* Extend cache if needed. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = cache_cnt + 1; k <= n; ++k) {
        if (k == 1) {                 /* safety, although already set */
            cache[1] = (seq_t)2;
            continue;
        }

        uint64_t cnt = 0;
        uint64_t uk = (uint64_t)k;
        for (idx_t i = 1; i < k; ++i) {
            uint64_t ai = (uint64_t)cache[i];
            if (gcd64(ai, uk) == 1)
                ++cnt;
        }
        cache[k] = (seq_t)cnt;
    }

    cache_cnt = n;
    return cache[n];
}
