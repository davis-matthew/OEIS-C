/*
 * A205139  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205138) for which such j exists, and s(k)=k*(3k-1)/2, the
 *          k-th pentagonal number.
 * https://oeis.org/A205139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A205139: for each n≥1 find the smallest k≥2 such that some j<k satisfies
   n | (s(k) – s(j)), where s(k) = k*(3k–1)/2 is the k‑th pentagonal number.
   The function returns that j.  The algorithm stores the first occurrence of
   each pentagonal residue modulo n and stops when a residue repeats.  */
#include <stdlib.h>

static seq_t *cache   = NULL;   /* memoised a(n) values, index 0 unused */
static idx_t cache_size = 0;    /* current cache capacity (in terms of n) */

static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(*new_cache));
        if (!new_cache) {
            new_cache = cache;       /* keep old cache on failure */
            return;
        }
        for (idx_t i = cache_size; i < new_size; ++i) {
            new_cache[i] = 0;         /* default value (j≥1) */
        }
        cache = new_cache;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, safety only */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    /* array holding the first occurrence of each residue modulo n */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(*first));
    if (!first) return 0;              /* unlikely */

    for (idx_t k = 1; ; ++k) {
        /* compute s(k) = k*(3k-1)/2 in seq_t */
        seq_t s = ((seq_t)k * (3 * k - 1)) / 2;
        idx_t r = (idx_t)(s % (seq_t)n);   /* residue modulo n */

        if (first[r] == 0) {
            first[r] = k;                /* first time this residue appears */
        } else {
            cache[n] = first[r];          /* j found */
            free(first);
            return cache[n];
        }
    }
}
