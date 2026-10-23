/*
 * A132177  Value of A063882 at end of n-th generation of terms.
 * https://oeis.org/A132177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+c(n), where c(1)=1, c(2)=3, for n≥3, c(n)=2 if
 *            n≡0 or 2 (mod 5), else c(n)=1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A132177,
   using memoization to allow non‑monotonic calls. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache of computed terms */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t cache_size = 0;    /* highest n computed so far */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* grow the array if necessary */
    size_t needed = (size_t)n + 1;            /* index 0 unused */
    if (cache == NULL || needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < needed) new_cap = needed;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 5;
        } else {
            /* coefficient for the increment */
            seq_t inc = (i % 5 == 0 || i % 5 == 2) ? 2 : 1;
            cache[i] = cache[i - 1] * 2 + inc;
        }
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
