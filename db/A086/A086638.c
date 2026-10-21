/*
 * A086638  Numbers with no 11 or 000 in their binary expansion.
 * https://oeis.org/A086638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   generate the n-th term of OEIS sequence A086638 – numbers whose
 *            binary representation contains no '11' or '000' substrings
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_forbidden
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/has_forbidden.h"

static seq_t *cache = NULL;           /* dynamically grown cache of terms */
static idx_t cache_size = 0;          /* number of terms stored */
static idx_t cache_cap = 0;           /* capacity of the cache array */
static idx_t last_scanned = (idx_t)-1;/* last integer that was examined */

/* Ensure the cache has at least 'need' elements allocated. */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed; let the program abort */
        abort();
    }
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_size) {
        return cache[n];
    }

    /* Need more terms; increase cache size as necessary */
    ensure_cache(n + 1);

    idx_t x = last_scanned + 1; /* start scanning from the next integer */

    for (;;) {
        if (!has_forbidden(x)) {
            cache[cache_size++] = (seq_t)x; /* store the term */
            if (cache_size > n) {
                last_scanned = x;
                return cache[n];
            }
        }
        last_scanned = x;
        x++;
    }
}
