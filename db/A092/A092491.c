/*
 * A092491  a(n) = 2*A058094(n-2) - 5*A058094(n-3) + A058094(n-4) + a(n-1) for
 *          n >=4.
 * https://oeis.org/A092491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=6; for n>6:
 *            a(n)=6*a(n-1)-11*a(n-2)+9*a(n-3)-4*a(n-4)-4*a(n-5)+a(n-6)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Memoisation cache (0‑based indices correspond to a(n)=cache[n-1]) */
static seq_t *cache = NULL;
static idx_t cached_len = 0;   /* number of stored terms (cache[0] = a(1)) */
static idx_t cache_cap = 0;    /* allocated capacity of cache */

/* Ensure at least `needed` slots are allocated */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Build cache up to and including term with 0‑based index `idx` */
static void build_cache_up_to(idx_t idx)
{
    if (idx < cached_len) return;
    ensure_cache_cap(idx + 1);

    if (cached_len == 0) {
        cache[0] = 0; /* a(1) */
        cache[1] = 0; /* a(2) */
        cache[2] = 0; /* a(3) */
        cache[3] = 0; /* a(4) */
        cache[4] = 1; /* a(5) */
        cache[5] = 6; /* a(6) */
        cached_len = 6;
    }

    for (idx_t i = cached_len; i <= idx; ++i) {
        if (i < 6) continue;   /* already initialised */
        seq_t val = (seq_t)6 * cache[i - 1];
        val -= (seq_t)11 * cache[i - 2];
        val += (seq_t)9  * cache[i - 3];
        val -= (seq_t)4  * cache[i - 4];
        val -= (seq_t)4  * cache[i - 5];
        val += cache[i - 6];
        cache[i] = val;
    }
    cached_len = idx + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0 – return 0 */
    idx_t idx = n - 1;         /* convert to 0‑based index */
    build_cache_up_to(idx);
    return cache[idx];
}
