/*
 * A014899  a(n) = (16^(n+1) - 15*n - 16)/225.
 * https://oeis.org/A014899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^(n+1) - 15*n - 16)/225
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 16*a(n-1) + n,  a(0) = 0 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last = 0;          /* highest index already computed */

    if (!cache) {                    /* first call: initialise cache */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0; /* unlikely, but safe fallback */
        cache[0] = (seq_t)0;
        last = 0;
    }

    if (n > last) {                  /* grow cache up to n */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[last];   /* cannot grow, return last value */
        cache = tmp;

        for (idx_t i = last + 1; i <= n; ++i) {
            /* a(i) = 16 * a(i-1) + i */
            cache[i] = cache[i - 1] * (seq_t)16;
            cache[i] += (seq_t)i;
        }
        last = n;
    }

    return cache[n];
}
