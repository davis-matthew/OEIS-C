/*
 * A116747  Number of permutations of length n which avoid the patterns 1234,
 *          2413, 3241.
 * https://oeis.org/A116747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=72, a(6)=230, a(7)=689
 *            a(n) = 9*a(n-1) - 33*a(n-2) + 64*a(n-3) - 71*a(n-4) + 45*a(n-5)
 *            - 15*a(n-6) + 2*a(n-7) for n ≥ 8
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold terms up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate at least n+1 entries (index 0 unused) */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – abort as per typical library behaviour */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* The recurrence needs the first 7 values. */
static const seq_t init[8] = {0, 1, 2, 6, 21, 72, 230, 689};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for 0 but safe to return 0 */

    if (n < 8) return init[n];     /* base cases are trivial */

    ensure_cache(n);

    /* Initialise the first 7 entries (idempotent assignment) */
    for (size_t i = 1; i < 8; ++i) cache[i] = init[i];

    /* Compute forward up to n using the recurrence */
    for (size_t i = 8; i <= (size_t)n; ++i) {
        /* Compute positive and negative parts separately to avoid
           signed underflow on unsigned seq_t types. */
        seq_t pos = 9  * cache[i - 1] +
                    64 * cache[i - 3] +
                    45 * cache[i - 5] +
                    2  * cache[i - 7];
        seq_t neg = 33 * cache[i - 2] +
                    71 * cache[i - 4] +
                    15 * cache[i - 6];
        cache[i] = pos - neg;
    }

    return cache[n];
}
