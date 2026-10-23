/*
 * A169109  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the recurrence derived from G(t) =
 *            (t^25+2t^24+…+2t+1) / (1081t^25-46t^24- …-46t+1). Let p(n) = 1
 *            for n=0, 2 for 1≤n≤24, 1 for n=25, 0 otherwise. Then for all n≥0
 *            a(n) = p(n) + 46·∑_{i=1}^{24} a(n-i) – 1081·a(n-25) (products
 *            with a(k)=0 when k<0). Values for n<25 are obtained by...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms already computed */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* extend cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* insufficient memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t p;
        if (i == 0)          p = 1;
        else if (i <= 24)    p = 2;
        else if (i == 25)    p = 1;
        else                 p = 0;

        seq_t sum24 = 0;
        for (size_t j = 1; j <= 24; ++j) {
            if (i >= j) sum24 += cache[i - j];
        }
        seq_t sum25 = 0;
        if (i >= 25) sum25 = cache[i - 25];

        seq_t term = p;
        term += sum24 * ((seq_t)46);
        term -= sum25 * ((seq_t)1081);

        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
