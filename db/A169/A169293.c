/*
 * A169293  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n>0: a(n)=p_n + 38*∑_{k=1}^{28} a(n‑k) – 741*a(n‑29)
 *            where p_n = 1 if n==0 or n==29, 2 if 1≤n≤28, otherwise 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* number of elements allocated for cache */
    static idx_t  last   = 0;   /* highest index already computed */
    static bool   init   = false;

    /* Expand cache if necessary. */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 1 : cap << 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first element of the cache. */
    if (!init) {
        cache[0] = 1;   /* a(0) = 1 */
        last = 0;
        init = true;
    }

    /* Return already‑computed value. */
    if (last >= n)
        return cache[n];

    /* Compute terms from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* 38 * (a(i-1)+…+a(i-28)) */
        for (int k = 1; k <= 28; ++k) {
            if (i >= (idx_t)k)
                sum += (38 * cache[i - k]);
        }

        /* –741 * a(i-29) for i ≥ 29 */
        if (i >= 29)
            sum -= (741 * cache[i - 29]);

        /* Add p_i */
        seq_t p;
        if (i == 0 || i == 29)
            p = 1;
        else if (i <= 28)
            p = 2;
        else
            p = 0;

        cache[i] = sum + p;
    }

    last = n;
    return cache[n];
}
