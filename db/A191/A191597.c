/*
 * A191597  Expansion of x*(1+3*x)/ ( (1-4*x)*(1+x+x^2)).
 * https://oeis.org/A191597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191597:  a(n) = 3*a(n-1) + 3*a(n-2) + 4*a(n-3), 
 * a(0)=0, a(1)=1, a(2)=6.  The values grow fast but fit
 * in any of the supported seq_t types; we memoise them to handle
 * non‑sequential calls. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache for computed terms */
static size_t cache_len = 0;  /* number of valid entries in cache   */
static size_t cache_cap = 0;  /* allocated capacity (in seq_t elems) */

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 3;                       /* indices 0..2 */
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {                 /* allocation failure → return 0 */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* if already computed, just return */
    if ((idx_t)n < cache_len) {
        return cache[n];
    }

    /* grow cache if needed */
    while ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {                     /* reallocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t v = (seq_t)3 * cache[i - 1] +
                  (seq_t)3 * cache[i - 2] +
                  (seq_t)4 * cache[i - 3];
        cache[i] = v;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
