/*
 * A052496  Number of nonsingular n X n matrices over GF(8).
 * https://oeis.org/A052496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{i=0}^{n-1} (8^n - 8^i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n+1)=a(n)*8^n*(8^{n+1}-1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;     /* holds a(0)...a(k) */
    static size_t cache_cap = 0;    /* capacity of cache */
    static idx_t cached_n = 0;      /* highest n already computed */
    static seq_t pow_q = 0;         /* current 8^cached_n */
    static int initialized = 0;

    if (!initialized) {
        /* first call – initialise */
        pow_q = (seq_t)1;                 /* 8^0 */
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        cache[0] = (seq_t)1;              /* a(0) = 1 */
        cache_cap = n + 1;
        cached_n = 0;
        initialized = 1;
    } else if (n >= cache_cap) {
        /* grow cache to fit the requested n */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_n; i < n; ++i) {
        seq_t next_pow = pow_q * ((seq_t)8);                   /* 8^{i+1} */
        cache[i + 1] = cache[i] * pow_q * (next_pow - (seq_t)1);
        pow_q = next_pow;                                     /* update 8^new_n */
    }
    cached_n = n;

    return cache[n];
}
