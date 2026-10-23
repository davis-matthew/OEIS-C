/*
 * A169416  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A169416
 *  a(n) =   { 1 ,  n = 0
 *           { 2 ,  1 ≤ n ≤ 31
 *           { 1 ,  n = 32
 *           { 0 ,  n ≥ 33  }  + 17·∑_{k=1}^{31}a(n-k) – 153·a(n-32)
 *
 *  Recurrence obtained from the rational generating function
 *  (1+2∑_{k=1}^{31}x^k+x^32)/(1–17∑_{k=1}^{31}x^k+153x^32).
 */

#include <stdlib.h>

/* forward declaration from the build prelude */
seq_t a(idx_t n);

/* multiply a value by a small non‑negative constant using only
 * additions – this works for any seq_t (including arbitrary‑precision
 * classes) and avoids overflow on 64‑bit builds for the n values that
 * fit in 64 bits. */
static inline seq_t mul_const(seq_t v, unsigned int c)
{
    seq_t res = 0;
    for (unsigned int i = 0; i < c; ++i)
        res += v;
    return res;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms stored (up to cache_len-1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* ensure the cache can hold terms up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – return 0 as a sentinel value */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base case */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {

        /* coefficient from the numerator of the generating function */
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)1;
        else if (i <= 31)
            coeff = (seq_t)2;
        else if (i == 32)
            coeff = (seq_t)1;
        else
            coeff = (seq_t)0;

        /* Σ_{k=1}^{31} a(i-k)   (treat a(m)=0 for m<0) */
        seq_t sum17 = 0;
        for (idx_t k = 1; k <= 31 && i >= k; ++k)
            sum17 += cache[i - k];

        /* a(i) = coeff + 17*sum17 – 153*a(i-32) */
        seq_t val = mul_const(sum17, 17);
        if (i >= 32)
            val -= mul_const(cache[i - 32], 153);
        val += coeff;
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
