/*
 * A212826  Number of 0..5 arrays of length n with no adjacent pair equal to
 *          its immediately preceding adjacent pair, and new values introduced
 *          in 0..5 order.
 * https://oeis.org/A212826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 30*a(n-2) - 21*a(n-3) + 112*a(n-4) +
 *            63*a(n-5) - 119*a(n-6) - 120*a(n-7) - 30*a(n-8) for n > 11
 *            Initial values: a(1)=1, a(2)=2, a(3)=5, a(4)=13, a(5)=44,
 *            a(6)=168, a(7)=716, a(8)=3331, a(9)=16599, a(10)=87059,
 *            a(11)=473569
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[12] = {
    0,          /* a(0) unused */
    1, 2, 5, 13, 44,
    168, 716, 3331, 16599, 87059, 473569
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Allocate cache for indices 0..11 on first call */
    if (cache == NULL) {
        size_t init_cap = 12;                /* 0..11 inclusive */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failed */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init_terms[i];
        cache_size = 11;                     /* highest filled index */
    }

    if (n <= cache_size)        /* already computed */
        return cache[n];

    /* Ensure capacity for up to index n */
    if (n > cache_size) {
        size_t new_cap = n + 1;                /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failed */
        cache = tmp;
        /* new elements are uninitialized; they will be computed below */
    }

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 11) {                    /* use known initial values */
            cache[i] = init_terms[i];
            continue;
        }

        /* Positive part */
        seq_t pos = 11 * cache[i-1] + 112 * cache[i-4] + 63 * cache[i-5];

        /* Negative part */
        seq_t neg = 30 * cache[i-2] + 21 * cache[i-3]
                  + 119 * cache[i-6] + 120 * cache[i-7]
                  + 30 * cache[i-8];

        cache[i] = pos - neg;
    }

    cache_size = n;
    return cache[n];
}
