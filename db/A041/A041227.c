/*
 * A041227  Denominators of continued fraction convergents to sqrt(125).
 * https://oeis.org/A041227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1364*a(n-5) + a(n-10) (n ≥ 10) Initial terms: a(0)=1,
 *            a(1)=5, a(2)=6, a(3)=11, a(4)=61, a(5)=1353, a(6)=6826,
 *            a(7)=8179, a(8)=15005, a(9)=83204
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Pre‑computed first ten terms */
static const seq_t init_terms[10] = {
    (seq_t)1,      (seq_t)5,      (seq_t)6,      (seq_t)11,
    (seq_t)61,     (seq_t)1353,   (seq_t)6826,   (seq_t)8179,
    (seq_t)15005,  (seq_t)83204
};

/* Memoisation cache */
static seq_t  *cache      = NULL;   /* dynamic array of computed terms */
static size_t  cache_cap  = 0;      /* allocated capacity */
static idx_t   cache_len  = 0;      /* number of terms currently stored */

/* Ensure the cache can hold at least 'need' entries */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = need;
    if (new_cap < 64) new_cap = 64;           /* minimum block size   */
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (a(0) = 1) */
seq_t a(idx_t n)
{
    /* Small indices are returned directly */
    if (n < 10)
        return init_terms[n];

    /* Grow the cache if necessary */
    if (n >= cache_len)
    {
        ensure_cache((size_t)(n + 1));

        /* Initialise the cache with the first ten values if not done yet */
        if (cache_len < 10)
        {
            for (idx_t i = cache_len; i < 10; ++i)
                cache[i] = init_terms[i];
            cache_len = 10;
        }

        /* Compute all missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* a(i) = 1364*a(i-5) + a(i-10) */
            seq_t term = cache[i - 5] * (seq_t)1364 + cache[i - 10];
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
