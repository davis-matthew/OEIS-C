/*
 * A120463  Expansion of x*(1+x+2*x^3) / ((x-1)*(1+x)*(3*x^2-1)).
 * https://oeis.org/A120463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 3*a(n-4) for n >= 5 with a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=4, a(4)=6
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* G.f.: x*(1+x+2*x^3) / ((x-1)*(1+x)*(3*x^2-1)) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* array capacity */
    static size_t last   = 0;   /* highest index computed */

    size_t i = (size_t)n;

    /* initial allocation and seed of first 5 terms */
    if (cached < 5) {
        size_t newlen = (i + 1 > 5 ? i + 1 : 5);
        seq_t *tmp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure – safe fallback */
        cache = tmp;
        cached = newlen;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)6;
        last = 4;
    }

    /* grow cache to at least cover requested index */
    if (i + 1 > cached) {
        size_t newlen = i + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cached = newlen;
    }

    /* compute missing terms up to i */
    for (size_t k = last + 1; k <= i; ++k) {
        if (k <= 4) {                /* base terms already set */
            continue;
        }
        /* recurrence: a(k) = 4*a(k-2) - 3*a(k-4) */
        seq_t term = (seq_t)4 * cache[k - 2] - (seq_t)3 * cache[k - 4];
        cache[k] = term;
    }
    last = i;
    return cache[i];
}
