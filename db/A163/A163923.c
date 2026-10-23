/*
 * A163923  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163923 – Number of reduced words of length n in a Coxeter
 * group on 7 generators with relations (S_i)^2 = (S_i S_j)^6 = I.
 *
 * It satisfies the linear recurrence
 *   a(n) = 5*a(n-1)+5*a(n-2)+5*a(n-3)+5*a(n-4)+5*a(n-5)-15*a(n-6)
 *   for n ≥ 7, with the following initial values:
 *     a(0)=1, a(1)=7, a(2)=42, a(3)=252, a(4)=1512, a(5)=9072,
 *   and for n = 6 we add the constant term from the numerator of the
 *   generating function:
 *     a(6) = 5*(a5+…+a1) - 15*a0 + 1 .
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms          */
static size_t cache_size = 0;        /* current allocated size           */
static idx_t  last_computed = 0;     /* largest index that has been cached */

static const seq_t init_vals[6] = {
    (seq_t)1,
    (seq_t)7,
    (seq_t)42,
    (seq_t)252,
    (seq_t)1512,
    (seq_t)9072
};

/* Make sure cache has room for indices 0..n inclusive.           */
static void ensure_cache(size_t n) {
    if (cache_size > n)
        return;                       /* already large enough */

    size_t new_size = cache_size ? cache_size << 1 : 8;
    while (new_size <= n)
        new_size <<= 1;                /* at least n+1 slots */

    seq_t *new_mem = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_mem) abort();             /* fatal error if memory fails */

    cache = new_mem;
    cache_size = new_size;
}

/* Return the n‑th term of the sequence.                        */
seq_t a(idx_t n)
{
    if (n < 6)                           /* base values for 0..5 */
        return init_vals[n];

    /* Expand the cache to hold at least indices 0..n.          */
    ensure_cache((size_t)n);

    /* Initialise the first six cached terms if not already done. */
    if (last_computed < 5) {
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        last_computed = 5;
    }

    /* Compute missing values in increasing order.              */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3]
                  + cache[i-4] + cache[i-5];

        seq_t val = (seq_t)5 * sum - (seq_t)15 * cache[i-6];

        /* Add the constant 1 for the term n = 6 (coefficient of t^6). */
        if (i == 6)
            val += (seq_t)1;

        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
