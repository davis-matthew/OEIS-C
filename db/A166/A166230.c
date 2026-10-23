/*
 * A166230  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*(a(n-1)+…+a(n-9)) – 820*a(n-10) + N_n where N_0 = 1,
 *            N_1 … N_9 = 2, N_10 = 1, and N_n = 0 for n > 10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* first ten terms, known from the sequence */
static const seq_t init[10] = {
    (seq_t)1ULL,
    (seq_t)42ULL,
    (seq_t)1722ULL,
    (seq_t)70602ULL,
    (seq_t)2894682ULL,
    (seq_t)118681962ULL,
    (seq_t)4865960442ULL,
    (seq_t)199504378122ULL,
    (seq_t)8179679503002ULL,
    (seq_t)335366859623082ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t len     = 0;   /* number of terms stored (valid indices 0 .. len-1) */
    static size_t capacity = 0;  /* allocated slots */

    /* initialise cache with the first ten terms */
    if (!cache) {
        capacity = 10;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        len = 10;
    }

    /* return if already computed */
    if (n < len)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* compute terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        /* sum the last nine terms */
        for (int k = 1; k <= 9; ++k)
            if (i >= (size_t)k)
                sum += cache[i - k];

        seq_t term = sum * (seq_t)40;
        if (i >= 10)
            term -= cache[i - 10] * (seq_t)820;

        /* add the N_n term for n = 10 (others are 0; n<10 are already in cache) */
        if (i == 10)
            term += (seq_t)1;

        cache[i] = term;
    }

    len = (size_t)n + 1;
    return cache[n];
}
