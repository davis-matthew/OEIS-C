/*
 * A042496  Numerators of continued fraction convergents to sqrt(776).
 * https://oeis.org/A042496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A042496: Numerators of continued fraction convergents to sqrt(776) 
   Recurrence: a(n) = 390*a(n-4) - a(n-8)  for n >= 8
   Initial values:
     a(0) = 27, a(1) = 28, a(2) = 167, a(3) = 195,
     a(4) = 10697, a(5) = 10892, a(6) = 65157, a(7) = 76049
*/

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_terms[8] = {27, 28, 167, 195,
                                    10697, 10892, 65157, 76049};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Expand the cache to at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: abort to avoid undefined behaviour */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle indices below the offset directly */
    if (n < 8) return init_terms[(size_t)n];

    /* Initialize cache on first use */
    if (cache_len == 0) {
        ensure_capacity(8);
        for (size_t i = 0; i < 8; ++i) cache[i] = init_terms[i];
        cache_len = 8;
    }

    /* Extend cache to cover index 'n' */
    if ((size_t)n + 1 > cache_len) {
        ensure_capacity((size_t)n + 1);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = 390 * cache[i - 4] - cache[i - 8];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
