/*
 * A036245  Numerator of fraction equal to the continued fraction [ 0, 1, 4,
 *          ..., n^2 ].
 * https://oeis.org/A036245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = n^2 * a(n-1) + a(n-2) for n > 2 a(1) = 1, a(2) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;       /* array indexed starting at 1 */
    static idx_t cache_cap = 0;       /* capacity of the array (bytes: sizeof(seq_t)*cache_cap) */
    static idx_t computed = 0;        /* highest index that has been computed */

    /* Ensure capacity for at least n+1 elements (indices 0..n) */
    if (cache_cap <= n) {
        idx_t new_cap = (n + 1) * 2;          /* grow factor */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();                   /* memory failure */
        /* zero‑initialize newly allocated slots */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base values once */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        computed = 2;
    }

    /* Compute missing values up to n */
    while (computed < n) {
        ++computed;
        seq_t prev1 = cache[computed - 1];
        seq_t prev2 = cache[computed - 2];
        seq_t i_seq = (seq_t)computed;
        seq_t i_sq = i_seq * i_seq;              /* n^2 as seq_t */
        cache[computed] = i_sq * prev1 + prev2;  /* recurrence */
    }

    return cache[n];
}
