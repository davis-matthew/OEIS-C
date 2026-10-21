/*
 * A221541  Number of 0..7 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 4*a(n-2) + 5*a(n-3) + 20*a(n-4) * + 20*a(n-5)
 *            + 23*a(n-6) - 6*a(n-7) + 3*a(n-8) * (empirical recurrence for n
 *            >= 9) * Initial values: a(1)=0, a(2)=7, a(3)=44, a(4)=350, *
 *            a(5)=2658, a(6)=20386, a(7)=156098, a(8)=1195561
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* start at index 0, unused */
static idx_t  cache_last = 0;        /* largest index computed so far */
static size_t cache_cap  = 0;        /* allocated capacity */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (!cache) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – caller will crash */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialize the cache with the first 8 terms. */
static void init_cache(void)
{
    if (!cache) {
        cache_cap = 9u;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure */
        cache[0] = 0;        /* unused */
        cache[1] = 0;        /* a(1) */
        cache[2] = 7;        /* a(2) */
        cache[3] = 44;       /* a(3) */
        cache[4] = 350;      /* a(4) */
        cache[5] = 2658;     /* a(5) */
        cache[6] = 20386;    /* a(6) */
        cache[7] = 156098;   /* a(7) */
        cache[8] = 1195561;  /* a(8) */
        cache_last = 8;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1, but defensive */

    if (!cache) init_cache();

    if ((idx_t)cache_last >= n) return cache[n];

    ensure_cache_size(n);

    for (idx_t k = cache_last + 1; k <= n; ++k) {
        seq_t v = 7 * cache[k-1];
        v += 4 * cache[k-2];
        v += 5 * cache[k-3];
        v += 20 * cache[k-4];
        v += 20 * cache[k-5];
        v += 23 * cache[k-6];
        v -= 6 * cache[k-7];
        v += 3 * cache[k-8];
        cache[k] = v;
    }
    cache_last = n;
    return cache[n];
}
