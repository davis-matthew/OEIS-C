/*
 * A056911  Odd squarefree numbers.
 * https://oeis.org/A056911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 9170, u32 n <= 9170, u64 n <= 9170,
 *            u128 n <= 9170, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th odd squarefree number by incremental search */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility header for the square‑free test */
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;      /* dynamically grown cache of terms */
static size_t cache_size = 0;    /* number of terms actually stored */
static size_t cache_cap  = 0;    /* allocated capacity of the cache      */
static idx_t  candidate  = 1;    /* next integer to test (starts at 1)   */

/* Ensure that the cache contains at least n terms (n >= 1) */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_size) {
        return;                 /* already enough terms */
    }

    /* Allocate or grow the cache to at least n entries */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        while (cache_cap < n) {
            cache_cap <<= 1;    /* double the capacity */
        }
    }
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

    /* Scan successive integers, storing the odd square‑free ones */
    while (cache_size < n) {
        if ((candidate & 1ULL) && is_squarefree((seq_t)candidate)) {
            cache[cache_size++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* Return the n‑th odd squarefree number (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* undefined for n < 1, return 0 */
    }
    ensure_cache(n);
    return cache[n - 1];
}
