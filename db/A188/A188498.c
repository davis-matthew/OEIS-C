/*
 * A188498  Number of permutations p on the set [n] with the properties that
 *          abs(p(i)-i) <= 3 for all i, p(1) <= 2, and p(j) >= 2 for j=3,4.
 * https://oeis.org/A188498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum_{k=1}^{14} d_k * a(n-k)  for n >= 14
 * where the coefficients d_k are obtained from the denominator
 * of the generating function:
 *    -1*a(n) +  2*a(n-1)+ 2*a(n-2)+ 0*a(n-3)+10*a(n-4)+ 8*a(n-5)
 *  - 2*a(n-6)-16*a(n-7)-10*a(n-8)- 2*a(n-9)+ 4*a(n-10)+ 2*a(n-11)
 *    0*a(n-12)+ 2*a(n-13)+ 1*a(n-14) = 0
 *   =>  a(n) = 2*a(n-1)+2*a(n-2)+0*a(n-3)+10*a(n-4)+8*a(n-5)
 *          -2*a(n-6)-16*a(n-7)-10*a(n-8)-2*a(n-9)+4*a(n-10)
 *          +2*a(n-11)+0*a(n-12)+2*a(n-13)+1*a(n-14)
 * The first 14 terms are provided explicitly.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity */
    static size_t cache_len  = 0;   /* number of terms stored */

    /* Initial 14 terms (a(0) … a(13)) */
    static const seq_t init_terms[14] = {
        (seq_t)0ULL,  (seq_t)1ULL,  (seq_t)2ULL,  (seq_t)3ULL,
        (seq_t)8ULL,  (seq_t)30ULL, (seq_t)102ULL, (seq_t)308ULL,
        (seq_t)905ULL,(seq_t)2744ULL,(seq_t)8473ULL,(seq_t)26112ULL,
        (seq_t)79924ULL,(seq_t)244204ULL
    };

    /* Coefficients d_k (k = 1 … 14) */
    static const int coeff[14] = {
        2,   2,   0,  10,   8,  -2, -16, -10,  -2,   4,   2,  0,   2,   1
    };

    /* Initialise cache with the 14 known terms if it hasn't been done already */
    if (cache_len == 0) {
        cache_cap = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure fallback */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init_terms[i];
        cache_len = cache_cap;
    }

    /* Return cached value if it already exists */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Ensure the cache has enough space for up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t pos = 0;     /* sum of positive contributions */
        seq_t neg = 0;     /* sum of negative contributions (as positive numbers) */

        /* Apply the recurrence */
        for (int k = 0; k < 14; ++k) {
            seq_t term = cache[i - (k + 1)];
            int c = coeff[k];
            if (c > 0)
                pos += term * (seq_t)c;
            else if (c < 0)
                neg += term * (seq_t)(-c);
        }
        cache[i] = pos - neg;   /* pos >= neg by construction of the sequence */
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
