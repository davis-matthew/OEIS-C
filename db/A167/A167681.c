/*
 * A167681  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A167681
   Let a(0)=1.
   For 1 ≤ n ≤ 14:
       a(n)=2 + 19⋅∑_{i=1}^{n} a(n‑i)
   For n=15:
       a(15)=1 + 19⋅∑_{i=1}^{14} a(15‑i) – 190⋅a(0)
   For n ≥ 16:
       a(n)=19⋅∑_{i=1}^{14} a(n‑i) – 190⋅a(n‑15)
   (All sums are over existing indices, missing terms are treated as 0.)   */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed values   */
    static size_t  cap   = 0;     /* capacity of cache          */
    static idx_t   last  = (idx_t)-1;  /* highest computed index  */
    static int     inited = 0;           /* whether the 0..15 init done */

    /* Helper: ensure capacity for indices up to n (inclusive).  */
    if (cache == NULL || cap <= n) {
        size_t new_cap = (cap == 0) ? 64 : cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise a(0)..a(15) on first use. */
    if (!inited) {
        seq_t init[16];
        init[0] = (seq_t)1;   /* a(0) */
        for (idx_t m = 1; m <= 15; ++m) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 14 && i <= m; ++i) sum += init[m - i];
            seq_t coef = (m == 0 || m == 15) ? 1 : 2;
            seq_t val  = coef + (seq_t)19 * sum;
            if (m == 15) val -= (seq_t)190 * init[0];
            init[m] = val;
        }
        /* copy to cache */
        for (idx_t i = 0; i <= 15; ++i) cache[i] = init[i];
        last = 15;
        inited = 1;
    }

    /* Return cached value if already computed. */
    if (n <= last) return cache[n];

    /* Compute terms from last+1 up to n (recurrence for n ≥ 16). */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 15) {
            /* Should already be filled, but keep for safety. */
            continue;
        }
        seq_t sum = 0;
        for (idx_t j = 1; j <= 14; ++j) sum += cache[i - j];
        seq_t val = (seq_t)19 * sum - (seq_t)190 * cache[i - 15];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
