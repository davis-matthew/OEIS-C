/*
 * A041565  Denominators of continued fraction convergents to sqrt(300).
 * https://oeis.org/A041565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Denominators of continued fraction convergents to sqrt(300)
 * Recurrence: a(n) = 2702*a(n-4) – a(n-8)  for n > 7
 * Initial terms: a(0)=1, a(1)=3, a(2)=25, a(3)=78, a(4)=2677,
 * a(5)=8109, a(6)=67549, a(7)=210756
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of already computed terms   */
static size_t cache_cap = 0;         /* allocated capacity of the cache   */
static size_t cache_size = 0;        /* number of terms stored in cache   */

/* The first eight terms of the sequence.  */
static const seq_t init_terms[8] = {
    1, 3, 25, 78, 2677, 8109, 67549, 210756
};

/* Ensure that the cache can hold indices up to 'needed' (inclusive).   */
static void ensure_cache_size(size_t needed)
{
    if (cache_cap > needed) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= needed) new_cap <<= 1;

    /* In C++ an explicit cast is required for the result of realloc. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;       /* indices used in this program are small */

    /* Initialise cache on the first call. */
    if (cache_size == 0) {
        ensure_cache_size(8);
        for (size_t i = 0; i < 8; ++i) cache[i] = init_terms[i];
        cache_size = 8;
    }

    /* Cached result available?  */
    if (idx < cache_size) return cache[idx];

    /* Grow the cache to hold the requested index. */
    ensure_cache_size(idx);

    /* Compute all missing terms up to idx. */
    for (size_t i = cache_size; i <= idx; ++i) {
        if (i < 8) {
            cache[i] = init_terms[i];
        } else {
            /* a(i) = 2702*a(i-4) – a(i-8)  for i ≥ 8. */
            cache[i] = (2702 * cache[i - 4]) - cache[i - 8];
        }
    }

    cache_size = idx + 1;
    return cache[idx];
}
