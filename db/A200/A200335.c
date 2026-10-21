/*
 * A200335  a(n) = sqrt((A200216(n)+1)/5).
 * https://oeis.org/A200335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1860497*a(n-1) + 1860497*a(n-2) - a(n-3); Initial values:
 *            a(1)=137, a(2)=253772063, a(3)=472142416783537
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
        if (!new_cache) exit(1);          /* allocation failure */
        cache = new_cache;
        cache_cap = (size_t)needed;
    }
}

/* Return the n‑th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    /* Expand cache if necessary. */
    if (n > cache_cap) {
        size_t old_cap = cache_cap;
        ensure_capacity(n);
        for (size_t i = old_cap; i < (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)137;
            } else if (i == 1) {
                cache[i] = (seq_t)253772063;
            } else if (i == 2) {
                cache[i] = (seq_t)472142416783537;
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];
                /* recurrence: 1860497*a(n-1) + 1860497*a(n-2) - a(n-3) */
                cache[i] = (seq_t)1860497 * a1
                           + (seq_t)1860497 * a2
                           - a3;
            }
        }
    }
    return cache[n - 1];
}
