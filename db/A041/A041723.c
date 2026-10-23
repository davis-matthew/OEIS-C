/*
 * A041723  Denominators of continued fraction convergents to sqrt(381).
 * https://oeis.org/A041723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2030*a(n-6) – a(n-12) for n > 11, a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=25, a(4)=27, a(5)=52, a(6)=2003, a(7)=2055, a(8)=4058,
 *            a(9)=50751, a(10)=54809, a(11)=105560
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* cached values */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of entries currently stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure that index n can be accessed in the cache. */
static void ensure_cache(idx_t n)
{
    /* already large enough */
    if (n < (idx_t)cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the nth term of A041723. */
seq_t a(idx_t n)
{
    if (n < (idx_t)cache_size) return cache[(size_t)n];

    ensure_cache(n);

    /* initial terms for indices 0..11 */
    static const seq_t init[12] = {
        1,   1,   2,   25,   27,   52,
        2003,2055,4058,50751,54809,105560
    };

    /* fill missing initial values */
    if (cache_size < 12) {
        size_t upto = 12;
        if ((size_t)n + 1 < upto) upto = (size_t)n + 1;
        for (size_t i = cache_size; i < upto; ++i) {
            cache[i] = init[i];
        }
        cache_size = upto;
        if (n < 12) return cache[(size_t)n];
    }

    /* compute additional terms using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* use 128‑bit intermediate to avoid overflow for 64‑bit seq_t */
        cache[i] = (seq_t)((unsigned __int128)2030 * cache[i - 6] - cache[i - 12]);
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
