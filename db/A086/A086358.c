/*
 * A086358  Digital root of n!.
 * https://oeis.org/A086358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of n! (i.e., n! modulo 9, with 0 replaced by
 *            9)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;       /* array of a(k) for k=0..cached_n */
    static idx_t cached_n = 0;        /* highest index computed in cache */
    static seq_t fact_mod9 = 1;       /* n! mod 9 (factorial value modulo 9) */
    idx_t i;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed – unsafe fallback */
        cache[0] = 1;                /* 0! = 1, digital root is 1 */
        cached_n = 0;
        fact_mod9 = 1;               /* 0! ≡ 1 (mod 9) */
    }

    /* If requested index is already cached, return it immediately */
    if (n <= cached_n) {
        return cache[n];
    }

    /* Extend cache up to the requested index */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return 0;   /* allocation failed – unsafe fallback */
    cache = tmp;

    /* Compute factorial modulo 9 for new indices */
    for (i = cached_n + 1; i <= n; ++i) {
        /* Multiply by i (mod 9) and take modulo 9 again */
        fact_mod9 = (seq_t)((fact_mod9 * ((seq_t)(i % 9u))) % 9u);
        seq_t val = fact_mod9;
        if (val == 0) val = 9u;              /* digital root convention */
        cache[i] = val;
    }

    cached_n = n;
    return cache[n];
}
