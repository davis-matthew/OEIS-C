/*
 * A169559  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=18, a(2)=306, … (see OEIS A169559) For n≥35: a(n) =
 *            16*a(n-1)+16*a(n-2)+…+16*a(n-34) – 136*a(n-35) The first 35
 *            terms are obtained from the recurrence a(n) = Σ_{k=1}^{n}
 *            16*a(n-k) + p_n (p_0=1, p_k=2 for 1≤k≤34)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies only on standard C/allocation operations and
   the integer types seq_t and idx_t supplied by the build system. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache   = NULL;   /* array of a[0..max]     */
    static size_t cache_cap = 0;    /* allocated capacity     */
    static idx_t  max_computed = 0; /* highest index computed */

    /* initialise or grow the cache on demand */
    if (cache == NULL) {
        cache_cap = 64;                                 /* initial size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute the first 35 terms (0 … 34) once */
    if (max_computed == 0) {
        /* a(0) = 1 */
        cache[0] = (seq_t)1;

        /* build subsequent terms using the short‑range recurrence
           a(n) = 16*Σ_{k=1..n} a(n-k) + p_n   (p_0=1, p_k=2 for k≥1, n≤34) */
        for (idx_t i = 1; i <= 34; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += (seq_t)16 * cache[i - k];
            }
            /* p_i = 2 for 1 ≤ i ≤ 34 */
            cache[i] = sum + (seq_t)2;
        }
        max_computed = 34;
    }

    /* return a cached value if already computed */
    if (n <= max_computed) return cache[n];

    /* extend the cache up to the requested index using the full recurrence */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        /* 16 times the last 34 terms */
        for (idx_t k = 1; k <= 34; ++k) {
            sum += (seq_t)16 * cache[cur - k];
        }
        /* subtract 136 times the term 35 places back */
        sum -= (seq_t)136 * cache[cur - 35];
        cache[cur] = sum;
    }
    max_computed = n;
    return cache[n];
}
