/*
 * A064805  Numbers k such that phi(k) > phi(k+2).
 * https://oeis.org/A064805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) are the integers k such that φ(k) > φ(k+2)
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_phi.h"

static idx_t cached = 0;              /* number of terms stored */
static idx_t last_checked = 1;        /* next k to test */
static seq_t *cache = NULL;           /* dynamic array of found k's */
static size_t cache_cap = 0;          /* current allocated capacity */

/* Append a new term to the cache, growing the buffer if necessary. */
static void add_term(idx_t v)
{
    if (cached == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);        /* unrecoverable memory error */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cached++] = (seq_t)v;
}

/* Return the n‑th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, but keep for safety */

    /* Grow cache until we have at least n terms. */
    while (cached < n) {
        seq_t phi_k  = _phi(last_checked);
        seq_t phi_k2 = _phi(last_checked + 2);
        if (phi_k > phi_k2) {
            add_term(last_checked);
        }
        ++last_checked;
    }
    return cache[n - 1];
}
