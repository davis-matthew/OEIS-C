/*
 * A120176  a(1)=8; a(n)=floor((41+sum(a(1) to a(n-1)))/5).
 * https://oeis.org/A120176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8; a(n) = floor((41 + sum_{k=1}^{n-1} a(k)) / 5) for n ≥
 *            2
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A120176. */
#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for n < 1 */

    /* Static cache for 1‑based indexing: cache[i] == a(i+1) */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;   /* number of terms already stored */
    static seq_t sum     = (seq_t)0; /* cumulative sum of stored terms */

    /* --- Allocate initial cache if necessary --- */
    if (cache_used == 0) {
        cache_cap = 16;             /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;/* allocation failure */
    }

    /* --- Grow cache to hold at least n terms --- */
    while (cache_used < (size_t)n) {
        if (cache_used == cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;/* realloc failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term */
        seq_t value;
        if (cache_used == 0) {
            value = (seq_t)8;        /* a(1) */
        } else {
            value = ((seq_t)41 + sum) / (seq_t)5;
        }

        cache[cache_used] = value;
        sum += value;
        cache_used++;
    }

    /* Return the requested term (1‑based index) */
    return cache[(size_t)n - 1];
}
