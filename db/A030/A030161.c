/*
 * A030161  Numbers k such that in k and k^3 the parity of digits alternates.
 * https://oeis.org/A030161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   has_alternating_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A030161: Numbers k such that in k and k^3 the parity of digits alternates.
 * This implementation generates terms on demand by brute‑force enumeration
 * of successive integers, keeping a cache of already found values.
 * The check for alternating parity is performed on k itself and on k^3.
 * The cache is expanded lazily, so subsequent calls to a() in any order
 * are fast because already computed terms are reused.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_alternating_parity.h"

/* Cache of computed sequence terms (1‑based indexing). */
static seq_t *cache     = NULL;   /* array of terms */
static size_t  cache_cap = 0;     /* allocated capacity */
static size_t  cache_len = 0;     /* number of terms stored */
static uint64_t last_checked = 0; /* largest integer already tested */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but avoid UB */

    /* Ensure the cache at least contains the first term (0). */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) __builtin_trap();     /* out of memory */
        cache[0] = (seq_t)0;              /* a(1) = 0 */
        cache_len = 1;
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        seq_t k = (seq_t)last_checked;
        if (!has_alternating_parity(k)) continue;
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        if (!has_alternating_parity(k3)) continue;

        /* Add the qualifying k to the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) __builtin_trap();   /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = k;
    }

    return cache[n - 1];   /* return the nth term (1‑based). */
}
