/*
 * A032743  Super-2 Numbers (2 * n^2 contains substring '22' in its decimal
 *          expansion).
 * https://oeis.org/A032743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Super‑2 Numbers (OEIS A032743).
   a(n) is the n‑th integer (n ≥ 1) whose 2·n² contains the decimal
   substring "22".  The function searches sequentially for candidate
   numbers, keeping a static cache so repeated calls return the correct
   value even if n is supplied in arbitrary order. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Check whether 2·x² (in decimal) contains the substring "22". */
static bool has_22(seq_t x)
{
    seq_t y = x * x;   /* x² */
    y *= 2;            /* 2·x² */

    int prev = -1;     /* previous (less‑significant) digit, -1 means none */
    while (y > 0) {
        int d = (int)(y % 10);   /* current digit */
        if (d == 2 && prev == 2) return true;
        prev = d;
        y /= 10;
    }
    return false;
}

/* Return the n-th Super‑2 Number. */
seq_t a(idx_t n)
{
    /* Static cache for already found terms. */
    static seq_t *cache   = NULL;
    static idx_t   cache_len = 0;
    static idx_t   cache_cap = 0;
    static idx_t   next_candidate = 1;   /* next integer to test */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until the requested index is filled. */
    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (has_22(val)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
