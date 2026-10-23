/*
 * A133439  a(1)=1; a(n) = Sum_{1<=k<=n, gcd(k,n)=1} a(floor(sqrt(k))).
 * https://oeis.org/A133439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=Σ_{1≤k≤n, gcd(k,n)=1} a(⌊√k⌋)  (OEIS A133439) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* zero‑based cache, cache[0] == a(1) */
static idx_t  max_cached = 0; /* largest n already cached (1‑based) */
static size_t cap = 0;        /* allocated size of the cache */

/* ---------- integer square root (floor) for 64‑bit inputs ---------- */
static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0) return 0;
    idx_t lo = 1, hi = n, res = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid <= n / mid) {   /* mid*mid ≤ n, no overflow here */
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* enlarge the cache capacity to at least new_cap, zero‑initialising the new part */
static void grow_cache(size_t new_cap) {
    if (new_cap <= cap) return;
    size_t old_cap = cap;
    /* double until we reach at least the requested size */
    while (cap < new_cap) cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) abort();   /* out of memory */
    /* zero the newly allocated tail */
    if (new_cache != cache) {
        memset(new_cache + old_cap, 0, (cap - old_cap) * sizeof(seq_t));
    } else {
        memset(new_cache + old_cap, 0, (cap - old_cap) * sizeof(seq_t));
    }
    cache = new_cache;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for 0, safe fallback */

    /* on first call, initialise cache with a(1)=1 */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;           /* a(1) */
        max_cached = 1;
    }

    /* extend cache up to n if necessary */
    if (n > max_cached) {
        grow_cache((size_t)n);
        for (idx_t m = max_cached + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                if (gcd((seq_t)k, (seq_t)m) == (seq_t)1) {
                    idx_t r = isqrt_u64(k);      /* floor(sqrt(k)) ≥ 1 */
                    sum += cache[r - 1];          /* a(r) already cached */
                }
            }
            cache[m - 1] = sum;
        }
        max_cached = n;
    }
    return cache[n - 1];
}
