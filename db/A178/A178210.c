/*
 * A178210  Numbers that are not the sum of squares of distinct composite
 *          numbers.
 * https://oeis.org/A178210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 220, u16 n <= 493, u32 n <= 493, u64 n <= 493,
 *            u128 n <= 493, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A178210
 *  Numbers that are NOT the sum of squares of distinct composite numbers.
 *  The function a(idx_t n) returns the nth term (1‑based).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root (floor) for unsigned 64‑bit values */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* sequence cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t cache_limit = 0;

/* helper to recompute the cache up to at least needed terms */
static void recompute_cache(size_t needed)
{
    /* grow limit until we have enough non‑representable numbers */
    while (true) {
        /* if limit unset, initialize */
        if (cache_limit == 0)
            cache_limit = (idx_t)needed * 20 + 500;   /* heuristic start */

        /* representable flag array */
        size_t rep_size = (size_t)cache_limit + 1;
        char *rep = (char *)calloc(rep_size, 1);
        rep[0] = 1;                           /* 0 is achievable by empty sum */

        /* list of composite numbers up to sqrt(limit) */
        idx_t sq_lim = isqrt_idx(cache_limit);
        size_t sieve_size = (size_t)sq_lim + 1;
        unsigned char *is_prime =
            (unsigned char *)malloc(sieve_size);
        for (size_t i = 0; i < sieve_size; ++i) is_prime[i] = 1;
        if (sieve_size > 0) is_prime[0] = 0;
        if (sieve_size > 1) is_prime[1] = 0;
        for (size_t i = 2; i * i <= sq_lim; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= sq_lim; j += i)
                    is_prime[j] = 0;

        /* subset‑sum DP over composite squares */
        for (idx_t c = 4; c <= sq_lim; ++c) {
            if (!is_prime[(size_t)c]) {
                idx_t sq = c * c;
                if (sq > cache_limit) continue;
                for (idx_t s = cache_limit; s >= sq; --s)
                    if (rep[(size_t)(s - sq)]) rep[(size_t)s] = 1;
            }
        }
        free(is_prime);

        /* rebuild cache array */
        if (cache_cap < (size_t)cache_limit) {
            cache = (seq_t *)realloc(cache, (size_t)cache_limit * sizeof(seq_t));
            cache_cap = (size_t)cache_limit;
        }
        cache_len = 0;
        for (size_t i = 1; i <= cache_limit; ++i)
            if (!rep[i])
                cache[cache_len++] = (seq_t)i;

        free(rep);

        if (cache_len >= needed) return;

        /* need more terms: double the limit and recompute */
        cache_limit <<= 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1, but protection */
    if (cache_len < (size_t)n)
        recompute_cache(n);
    return cache[n - 1];
}
