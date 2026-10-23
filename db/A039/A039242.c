/*
 * A039242  Numbers whose base-12 representation has the same number of 3's
 *          and 7's.
 * https://oeis.org/A039242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer such that the number of 3's
 *            equals the number of 7's in its base‑12 representation
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_37
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_37.h"

static seq_t *cache = NULL;      /* stored terms, zero‑based */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t last_checked = 0;   /* next candidate to test */

/* Ensure cache has at least 'min_needed' entries allocated. */
static void grow_cache(size_t min_needed) {
    if (cache_cap >= min_needed)
        return;
    size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
    while (new_cap < min_needed)
        new_cap *= 2;
    /* realloc may return NULL only if new_cap==0, which cannot happen here. */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Public function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but return 0 safely */

    /* Extend cached terms until we have at least n terms. */
    while (cache_size < (size_t)n) {
        seq_t cand = last_checked;
        if (equal_37(cand)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = cand;
        }
        last_checked++;
    }

    return cache[n - 1];
}
