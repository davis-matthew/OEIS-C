/*
 * A041478  Numerators of continued fraction convergents to sqrt(255).
 * https://oeis.org/A041478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041478 – Numerators of continued fraction convergents to sqrt(255)
   Formula: a(n) = 32*a(n-2) - a(n-4)
   Initial values: a(0)=15, a(1)=16, a(2)=495, a(3)=511
   (Recurrence valid for n ≥ 4) */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache     = NULL;   /* array of values */
static idx_t  cache_cap = 0;      /* capacity in elements */
static idx_t  cache_len = 0;      /* number of computed elements */

/* ensure that values up to index n are computed and stored in cache */
static void
ensure_cache(idx_t n)
{
    /* trivial case: nothing needed for n < 4 (handled elsewhere) */
    if (n < 4) return;

    /* allocate or grow cache as necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;           /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc with NULL works as malloc */
        if (tmp == NULL) abort(); /* out of memory – non‑recoverable in this context */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize base terms if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)15;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)495;
        cache[3] = (seq_t)511;
        cache_len = 4;   /* we now have terms 0..3 available */
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)32 * cache[i - 2] - cache[i - 4];
    }
    cache_len = n + 1;
}

/* public function requested by the exercise */
seq_t a(idx_t n)
{
    if (n < 4) {
        /* handle small indices directly */
        switch (n) {
            case 0: return (seq_t)15;
            case 1: return (seq_t)16;
            case 2: return (seq_t)495;
            case 3: return (seq_t)511;
        }
    }
    ensure_cache(n);
    return cache[n];
}
