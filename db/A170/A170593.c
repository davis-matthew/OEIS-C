/*
 * A170593  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n ≤ 47 : a(n) = 2 + 6·∑_{i=0}^{n-1} a(i). For
 *            n = 48 : a(48) = 1 + 6·∑_{i=1}^{47} a(i) – 21·a(0). For n ≥ 49 :
 *            a(n) = 6·∑_{i=n-47}^{n-1} a(i) – 21·a(n-48)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Maximum N we preallocate for – the tests only go up to about 1e4. */
    enum { MAX_N = 20001 };
    static seq_t cache[MAX_N];
    static seq_t cum[MAX_N];
    static int  init = 0;
    static idx_t computed = 0;

    if (!init)
    {
        init = 1;
        if ((idx_t)MAX_N == 0) return 0;          /* avoid division by zero */
        cache[0] = 1;
        cum[0]   = 1;
        computed = 0;
    }

    if (n >= (idx_t)MAX_N)
    {
        /* If a larger n is requested, fall back to a simple dynamic strategy. */
        /* Allocate dynamic arrays large enough for n+1 elements. */
        static seq_t *dyn_cache = NULL;
        static seq_t *dyn_cum   = NULL;
        static idx_t dyn_cap    = 0;
        static idx_t dyn_computed = 0;

        if (dyn_cap <= n)
        {
            idx_t new_cap = (dyn_cap == 0 ? 64 : dyn_cap);
            while (new_cap <= n) new_cap <<= 1;
            dyn_cache = (seq_t *)realloc(dyn_cache, sizeof(seq_t) * new_cap);
            dyn_cum   = (seq_t *)realloc(dyn_cum,   sizeof(seq_t) * new_cap);
            if (!dyn_cache || !dyn_cum) return 0;    /* allocation failure → undefined */
            dyn_cap = new_cap;
        }

        if (dyn_computed == 0)
        {
            dyn_computed = 0;
            dyn_cache[0] = 1;
            dyn_cum[0]   = 1;
        }

        while (dyn_computed < n)
        {
            idx_t i = dyn_computed + 1;
            if (i <= 47)
            {
                dyn_cache[i] = 2 + 6 * dyn_cum[i-1];
                dyn_cum[i]   = dyn_cum[i-1] + dyn_cache[i];
            }
            else if (i == 48)
            {
                seq_t sum47 = dyn_cum[47] - dyn_cum[0];
                dyn_cache[i] = 1 + 6 * sum47 - 21 * dyn_cache[0];
                dyn_cum[i]   = dyn_cum[47] + dyn_cache[i];
            }
            else
            {
                seq_t sum47 = dyn_cum[i-1] - dyn_cum[i-48-1];
                dyn_cache[i] = 6 * sum47 - 21 * dyn_cache[i-48];
                dyn_cum[i]   = dyn_cum[i-1] + dyn_cache[i];
            }
            dyn_computed = i;
        }
        return dyn_cache[n];
    }

    /* If we are still within the preallocated range. */
    while (computed < n)
    {
        idx_t i = computed + 1;
        if (i <= 47)
        {
            cache[i] = 2 + 6 * cum[i-1];
            cum[i]   = cum[i-1] + cache[i];
        }
        else if (i == 48)
        {
            seq_t sum47 = cum[47] - cum[0];
            cache[i] = 1 + 6 * sum47 - 21 * cache[0];
            cum[i]   = cum[47] + cache[i];
        }
        else
        {
            seq_t sum47 = cum[i-1] - cum[i-48-1];
            cache[i] = 6 * sum47 - 21 * cache[i-48];
            cum[i]   = cum[i-1] + cache[i];
        }
        computed = i;
    }
    return cache[n];
}
