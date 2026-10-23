/*
 * A049349  Row sums of triangle A049325.
 * https://oeis.org/A049349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 16*a(n-3) + 16*a(n-4) for n>4;
 *            a(1)=1, a(2)=7, a(3)=29, a(4)=103
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;         /* cache[0] unused, cache[1..] holds a(n) */
static size_t cache_cap = 0;        /* allocated capacity (>= max_n+1) */
static idx_t max_n = 0;             /* largest n computed so far */

/* Ensure cache can hold index n (1‑based). Allocate or grow as needed. */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;      /* need space up to n inclusive */

    if (cache == NULL) {
        /* initial allocation: at least space for indices 0..7 */
        cache_cap = target > 8 ? target : 8;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();

        /* preset first four terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)29;
        cache[4] = (seq_t)103;
        max_n  = 4;
    }

    if (target <= cache_cap) return;  /* already sufficient */

    /* grow capacity to at least target */
    size_t new_cap = cache_cap;
    while (new_cap < target) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* realloc failed: keep old cache to avoid memory loss */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1, with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined; return 0 for safety */

    ensure_cache(n);

    if (n <= max_n) return cache[n];

    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* compute recurrence; accesses use k-1..k-4 which are valid for k>=5 */
        seq_t val = cache[k - 1] + (seq_t)6 * cache[k - 2]
                     + (seq_t)16 * cache[k - 3] + (seq_t)16 * cache[k - 4];
        cache[k] = val;
    }
    max_n = n;
    return cache[n];
}
