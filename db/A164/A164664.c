/*
 * A164664  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A164664 – Number of reduced words of length n in the Coxeter group
   on 28 generators S_i with relations S_i^2 = (S_i S_j)^7 = 1.
   The generating function is 
        (t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1) /
        (351*t^7 - 26*t^6 - 26*t^5 - 26*t^4 - 26*t^3 - 26*t^2 - 26*t + 1)
   This yields the linear recurrence (for n ≥ 7):
        a(n) = 26 · (a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6))
               - 351 · a(n-7)           (a(7) has an extra +1 term)
   With initial values:
        a(0)=1, a(1)=28, a(2)=756, a(3)=20412, a(4)=551124,
        a(5)=14880348, a(6)=401769396
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values */
    static const seq_t base[7] = {
        (seq_t)1ULL,
        (seq_t)28ULL,
        (seq_t)756ULL,
        (seq_t)20412ULL,
        (seq_t)551124ULL,
        (seq_t)14880348ULL,
        (seq_t)401769396ULL
    };

    /* cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t last_computed = 6;
    static int initialized = 0;

    /* returned directly for the first seven indices */
    if (n <= 6)
        return base[(size_t)n];

    /* initialize cache */
    if (!initialized) {
        cache_len = (n >= 7) ? (n + 1) : 7;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* unlikely to happen */
        for (size_t i = 0; i <= 6; ++i)
            cache[i] = base[i];
        last_computed = 6;
        initialized = 1;
    }

    /* enlarge cache if necessary */
    if (n + 1 > cache_len) {
        cache_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* allocation failure */
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 6; ++k)
            sum += cache[i - k];

        seq_t val = (seq_t)26 * sum - (seq_t)351 * cache[i - 7];
        if (i == 7)   /* special term from the numerator */
            val += (seq_t)1;

        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
