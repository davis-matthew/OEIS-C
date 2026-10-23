/*
 * A166615  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*∑_{k=1}^{11} a(n-k) – 351*a(n-12) + c(n), where
 *            c(0)=1, c(1..11)=2, c(12)=1, c(n)=0 for n>12. Offset 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t c[13] = {
    (seq_t)1ULL,  /* c(0) */
    (seq_t)2ULL,  /* c(1) */
    (seq_t)2ULL,  /* c(2) */
    (seq_t)2ULL,  /* c(3) */
    (seq_t)2ULL,  /* c(4) */
    (seq_t)2ULL,  /* c(5) */
    (seq_t)2ULL,  /* c(6) */
    (seq_t)2ULL,  /* c(7) */
    (seq_t)2ULL,  /* c(8) */
    (seq_t)2ULL,  /* c(9) */
    (seq_t)2ULL,  /* c(10) */
    (seq_t)2ULL,  /* c(11) */
    (seq_t)1ULL   /* c(12) */
};

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;
    static size_t cap    = 0;      /* allocated array size */
    static size_t computed = 0;    /* number of values already computed */

    /* Grow cache if necessary. */
    if (cap <= (size_t)n) {
        size_t new_cap = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) abort();
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cap = new_cap;
    }

    /* Compute missing terms. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t sum = 0;

        /* 26 * sum_{k=1}^{11} a(i-k)  (skip terms where i<k). */
        for (size_t k = 1; k <= 11; ++k) {
            if (i >= k)
                sum += cache[i - k] * (seq_t)26;
        }

        seq_t term = sum;

        /* Add the constant part c(i). */
        if (i <= 12)
            term += c[i];

        /* Subtract 351 * a(i-12) when i >= 12. */
        if (i >= 12)
            term -= cache[i - 12] * (seq_t)351;

        cache[i] = term;
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
