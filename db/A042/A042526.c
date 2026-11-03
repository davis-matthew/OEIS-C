/*
 * A042526  Numerators of continued fraction convergents to sqrt(792).
 * https://oeis.org/A042526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 394*a(n-2) - a(n-4) Initial values: a(0)=28, a(1)=197,
 *            a(2)=11060, a(3)=77617
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Return the n-th term of OEIS A042526 (numerators of convergents
   to sqrt(792)).  The function is memoised and works with any
   seq_t type provided by the build. */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current allocated capacity */
    static size_t computed = 0;    /* number of entries already computed */

    /* Ensure the cache can hold indices up to n */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        grow_seq_array(&cache, &cache_cap, need);
    }

    /* Initialise base cases once */
    if (computed == 0) {
        cache[0] = (seq_t)28;
        cache[1] = (seq_t)197;
        cache[2] = (seq_t)11060;
        cache[3] = (seq_t)77617;
        computed = 4;
    }

    /* Compute any missing terms up to n */
    if ((size_t)n >= computed) {
        const seq_t K = (seq_t)394;
        for (size_t i = computed; i <= (size_t)n; ++i) {
            seq_t t = cache[i - 2];
            t = t * K;
            t = t - cache[i - 4];
            cache[i] = t;
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
