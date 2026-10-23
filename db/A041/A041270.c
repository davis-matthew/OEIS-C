/*
 * A041270  Numerators of continued fraction convergents to sqrt(148).
 * https://oeis.org/A041270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 146*a(n-2) - a(n-4) Initial values: a(0)=12, a(1)=73,
 *            a(2)=1764, a(3)=10657
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    /* Initialise the cache with the first four terms on first call */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[0] = (seq_t)12;
        cache[1] = (seq_t)73;
        cache[2] = (seq_t)1764;
        cache[3] = (seq_t)10657;
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if ((idx_t)n < cache_len)
        return cache[n];

    /* Ensure the array large enough for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;           /* grow by factor of 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    const seq_t coeff = (seq_t)146;
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = coeff * cache[i - 2] - cache[i - 4];
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
