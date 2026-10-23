/*
 * A170530  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c_n + 39 * Σ_{k=1}^{min(45,n)} a(n-k) - 780 * a(n-46)
 *            (with a(m)=0 for m<0) where c_0 = 1, c_1..c_45 = 2, c_46 = 1,
 *            and c_n = 0 for n>46
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – use std::vector for automatic construction. */
    static std::vector<seq_t> cache;
    static idx_t computed = 0;

    if (cache.empty())
    {
        cache.resize(1);
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        computed = 0;
    }

    if (n >= cache.size())
        cache.resize(n + 1);

    for (idx_t i = computed + 1; i <= n; ++i)
    {
        /* coefficient c_i */
        seq_t coeff;
        if (i == 0 || i == 46)
            coeff = (seq_t)1;
        else if (i <= 45)
            coeff = (seq_t)2;
        else
            coeff = (seq_t)0;

        /* sum of the previous 45 terms (or fewer if i<45) */
        seq_t sum = 0;
        idx_t m = (i < 45 ? i : 45);
        for (idx_t k = 1; k <= m; ++k)
            sum += cache[i - k];

        seq_t val = coeff + (seq_t)39 * sum;

        if (i >= 46)
            val -= (seq_t)780 * cache[i - 46];

        cache[i] = val;
    }

    computed = n;
    return cache[n];
#else
    /* C implementation – use manual array allocation. */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* allocated elements */
    static idx_t computed = 0;

    if (!cache) {                 /* initialise cache with a(0) */
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;     /* a(0) = 1 */
        computed = 0;
    }

    /* grow array if needed */
    if ((size_t)n >= cap)
    {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();       /* out of memory – terminate */
        cache = tmp;
        cap = newcap;
    }

    for (idx_t i = computed + 1; i <= n; ++i)
    {
        /* coefficient c_i */
        seq_t coeff;
        if (i == 0 || i == 46)
            coeff = (seq_t)1;
        else if (i <= 45)
            coeff = (seq_t)2;
        else
            coeff = (seq_t)0;

        /* sum of the previous 45 terms */
        seq_t sum = 0;
        idx_t m = (i < 45 ? i : 45);
        for (idx_t k = 1; k <= m; ++k)
            sum += cache[i - k];

        seq_t val = coeff + (seq_t)39 * sum;

        if (i >= 46)
            val -= (seq_t)780 * cache[i - 46];

        cache[i] = val;
    }

    computed = n;
    return cache[n];
#endif
}
