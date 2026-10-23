/*
 * A138563  Beastly fax numbers: numbers containing the fax number of the
 *          Beast (667, one more than its regular number) in their decimal
 *          expansion.
 * https://oeis.org/A138563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_sub667
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Beastly fax numbers: numbers containing the decimal
   substring "667".  The function generates terms by brute‑force
   scanning the natural numbers and caching results so that calls
   in any order are handled efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_sub667.h"

static seq_t *cache = NULL;        /* cached terms */
static size_t cache_cap = 0;       /* allocated size of cache   */
static size_t cache_len = 0;       /* number of terms stored   */
static seq_t last_checked = 0;     /* last integer examined   */

/* Grow the cache so that it holds at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Allocate or grow the buffer as needed. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    seq_t cur = last_checked + 1;
    while (cache_len < (size_t)n) {
        if (has_sub667(cur)) {
            cache[cache_len++] = cur;
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
        }
        cur++;
    }
    last_checked = cur - 1;
}

/* Public interface: return the nth Beastly fax number. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for 0 */
    ensure_terms(n);
    return cache[n - 1];
}
