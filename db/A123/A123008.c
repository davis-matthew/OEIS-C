/*
 * A123008  Expansion of x*(1 + 3*x)/(1 - 2*x - 25*x^2).
 * https://oeis.org/A123008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 25*a(n-2), a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe */

    static seq_t *cache = NULL;     /* cached values a(1) .. a(cached_n) */
    static idx_t cached_n = 0;      /* highest index computed so far */

    if (n <= cached_n) {  /* already computed */
        return cache[n-1];
    }

    /* Allocate or grow cache to hold up to n */
    size_t new_size = (size_t)n * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_size);
    /* If realloc returned NULL, keep the old buffer and just return 0 (unlikely) */
    if (tmp == NULL) return 0;
    cache = tmp;

    /* Compute new terms */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        idx_t idx = i - 1; /* zero‑based index in cache */
        if (i == 1) {
            cache[idx] = (seq_t)1;
        } else if (i == 2) {
            cache[idx] = (seq_t)5;
        } else {
            cache[idx] = (seq_t)2 * cache[idx-1] + (seq_t)25 * cache[idx-2];
        }
    }
    cached_n = n;
    return cache[n-1];
}
