/*
 * A129339  Main diagonal of triangular array T: T(j,1) = 1 for ((j-1) mod 6)
 *          < 3, else 0; T(j,k) = T(j-1,k-1) + T(j,k-1) for 2 <= k <= j.
 * https://oeis.org/A129339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 6*a(n-3) for n > 4 with initial
 *            values a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 7
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 34, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache to allow arbitrary-order calls */
static seq_t *cache      = NULL;   /* cache[1..cache_n] holds computed terms */
static idx_t  cache_n    = 0;      /* maximum index cached (>=0) */
static size_t cache_cap  = 0;      /* allocated size of cache array */

/* grow cache array to hold values up to index n (inclusive) */
static void ensure_cache(idx_t n) {
    if (n == 0) return;           /* nothing needed for offset 0 */ 

    if (cache == NULL) {
        /* initialize cache with sufficient capacity */
        cache_cap = 5;              /* indices 0..4 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;         /* allocation failed */
        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache_n = 4;
    }

    if ((idx_t)n <= cache_n) return;

    /* enlarge capacity if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;           /* allocation failed, keep old cache */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms using the recurrence */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        seq_t t1 = 5 * cache[i - 1];   /* 5*a(i-1) */
        seq_t t2 = 9 * cache[i - 2];   /* 9*a(i-2) */
        seq_t t3 = 6 * cache[i - 3];   /* 6*a(i-3) */
        cache[i] = t1 + t3 - t2;       /* a(i) = 5*a(i-1) - 9*a(i-2) + 6*a(i-3) */
    }
    cache_n = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* sequence starts at offset 1 */
    ensure_cache(n);
    return (cache != NULL) ? cache[n] : (seq_t)0;
}
