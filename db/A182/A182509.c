/*
 * A182509  a(0)=0, a(1)=1, a(n)=(a(n-1) XOR n) + a(n-2).
 * https://oeis.org/A182509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = (a(n-1) XOR n) + a(n-2)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* table of previously computed values    */
static idx_t cache_cap = 0;      /* number of valid entries in *cache      */

/* Ensure cache contains values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;           /* already up‑to‑date */

    /* Allocate / enlarge the table */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failure */
    cache = tmp;

    /* Initialise missing base values */
    if (cache_cap == 0) {                /* first time */
        cache[0] = (seq_t)0;
        if (new_cap > 1) cache[1] = (seq_t)1;
    } else {
        if (cache_cap < 2 && new_cap > 1)   /* we don't have a[1] yet */
            cache[1] = (seq_t)1;
    }

    /* Compute new entries */
    idx_t start = (cache_cap >= 2) ? cache_cap : 2;
    for (idx_t i = start; i <= n; ++i)
        cache[i] = (cache[i-1] ^ (seq_t)i) + cache[i-2];

    cache_cap = new_cap;
}

/* Sequence function required by the framework                       */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);
    return cache[n];
}
