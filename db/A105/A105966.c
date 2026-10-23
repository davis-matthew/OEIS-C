/*
 * A105966  Expansion of A/B with A = (-1+x^15-x^10-x^9-x^8-2*x^5-x^4) and B =
 *          (x-1)*(x+1)*(x^2+x+1)*(x^4+x^3+x^2+x+1)*(x^4-x^3+x^2-x+1)*(x^8-x^7+x^5-x^4+x^3-x+1).
 * https://oeis.org/A105966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-5) + a(n-15) + a(n-20) for n>19.
   Initial terms a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1,
   a(5)=1, a(6)=0, a(7)=0, a(8)=1, a(9)=0,
   a(10)=0, a(11)=0, a(12)=0, a(13)=-1,
   a(14)=0, a(15)=0, a(16)=0, a(17)=0,
   a(18)=1, a(19)=1. */
#include <stdlib.h>

/* Precomputed initial values for indices 0 … 19 */
static const seq_t init_vals[20] = {
    1, 0, 0, 0, 1,
    1, 0, 0, 1, 0,
    0, 0, 0, -1, 0,
    0, 0, 0, 1, 1
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* First call: create a cache large enough to hold the requested
       index and all required initial values, then fill it. */
    if (cache == NULL) {
        /* Allocate at least 20 slots, but also large enough for n. */
        cache_size = (n + 1 > 20) ? (n + 1) : 20;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */

        /* Copy the 20 known initial terms. */
        for (idx_t i = 0; i < 20 && i < cache_size; ++i)
            cache[i] = init_vals[i];

        /* Compute any missing terms up to n using the recurrence. */
        for (idx_t i = 20; i <= n && i < cache_size; ++i)
            cache[i] = -cache[i - 5] + cache[i - 15] + cache[i - 20];

        /* The rest of the buffer (if any) is left zero‑initialised. */
    }

    /* If the request exceeds the current cache, grow it and continue
       filling with the recurrence. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;

        /* Compute newly needed values. */
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = -cache[i - 5] + cache[i - 15] + cache[i - 20];

        cache_size = new_size;
    }

    return cache[n];
}
