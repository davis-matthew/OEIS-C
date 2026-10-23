/*
 * A031514  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 16.
 * https://oeis.org/A031514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that the continued fraction of
 *            sqrt(k) has an even period and, after removing the last term of
 *            that period, the central remaining term equals 16
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   _floor_isqrt_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine generates terms on demand, caching all found values.
   It uses a simple brute‑force search over k ≥ 2, computing the
   continued‑fraction period for each k until the required number of
   terms has been collected. This is fast enough for n up to about 10 000
   within the allowed time limits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t  cache_len = 0;    /* number of stored terms */
static size_t  cache_cap = 0;    /* current capacity of cache */
static uint64_t next_k = 2;      /* next integer to test */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < need)
        newcap = need;

    seq_t *tmp = seq_realloc(cache, cache_cap, newcap);
    if (!tmp)
        abort();                     /* out of memory */
    cache     = tmp;
    cache_cap = newcap;
}

/* Store a new term in the cache. */
static void add_term(seq_t val)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Compute the continued‑fraction period of sqrt(k).
   Returns the period length and writes the period terms into the array
   'terms' (array must be allocated by the caller and large enough).
   The caller should call the function with a pointer to an array whose
   size is at least 512 (this suffices for all k considered).  The
   number of terms returned (p) is stored in *p_ret. */
static inline bool period_of_sqrt(uint64_t k, uint64_t *terms, size_t *p_ret)
{
    uint64_t a0 = _floor_isqrt_u64(k);
    if (a0 * a0 == k)          /* k is a perfect square */
        return false;

    uint64_t m   = 0;
    uint64_t d   = 1;
    uint64_t a   = a0;
    size_t    p  = 0;

    /* Compute period terms: a1 .. ap, where ap == 2*a0 */
    do {
        /* Advance to next term */
        m = d * a - m;
        /* (k - m*m) fits in uint64_t because m < sqrt(k) < 2^32 for all k
           tested; use 128‑bit for safety when k is near 2^64. */
        unsigned __int128 tmp = (unsigned __int128)(k - (unsigned __int128)m * m);
        d = (uint64_t)(tmp / d);
        a = (unsigned __int128)(a0 + m) / d;

        /* Store a */
        terms[p++] = a;
    } while (a != 2 * a0);

    *p_ret = p;
    return true;
}

/* Compute the sequence term a(n). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* Buffer for period terms; 512 is ample for all k in the search. */
    uint64_t *period = (uint64_t *)malloc(512 * sizeof(uint64_t));
    if (!period)
        abort();                 /* out of memory */

    /* Find enough terms. */
    for (; (idx_t)cache_len < n; ++next_k) {
        uint64_t k = next_k;
        size_t   p;

        if (!period_of_sqrt(k, period, &p))
            continue;             /* skip perfect squares */

        if ((p & 1) == 0) {       /* even period only */
            size_t central_idx = (p - 2) / 2;   /* remove last term */
            if (period[central_idx] == 16)
                add_term((seq_t)k);
        }
    }

    free(period);
    return cache[n - 1];
}
