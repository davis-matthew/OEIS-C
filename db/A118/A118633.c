/*
 * A118633  Start with 1 and repeatedly reverse the digits and add 37 to get
 *          the next term.
 * https://oeis.org/A118633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; a(n)=reverse(a(n-1))+37 for n>1
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse.h"

static seq_t *cache      = NULL;  /* array of computed terms */
static size_t cache_len  = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* allocated capacity */

/* Ensure the cache has at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first term. */
static void initialize_cache(void)
{
    if (cache_len > 0) return;      /* already initialised */
    ensure_capacity(1);
    cache[0] = (seq_t)1;            /* a(1) = 1 */
    cache_len = 1;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset is 1; undefined below */

    initialize_cache();

    if ((idx_t)cache_len >= n)      /* already computed */
        return cache[n - 1];

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i‑1)   */
        seq_t next = reverse(prev) + (seq_t)37; /* a(i) */
        ensure_capacity((size_t)i);
        cache[i - 1] = next;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
