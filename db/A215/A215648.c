/*
 * A215648  G.f. satisfies: A(x) = 1 + x*A(x)^2 + 3*x^2*A'(x)*A(x).
 * https://oeis.org/A215648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (3*k+1)*a(k)*a(n-k-1), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t capacity = 0;
static idx_t computed = 0;

/* Resize the cache to store at least (n+1) elements. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= capacity) return;          /* already big enough */

    size_t new_cap = capacity ? capacity : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;                 /* OOM – in production handle it */

    if (cache) {
        for (size_t i = 0; i < computed; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache   = new_cache;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n < computed) return cache[n];

    if (computed == 0) {                    /* initialise a(0) */
        cache[0] = (seq_t)1;
        computed = 1;
    }

    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;
        if (i == 0) {                       /* already set */
            cache[0] = 1;
            continue;
        }
        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = cache[k] * cache[i - 1 - k];
            prod *= (seq_t)(3 * k + 1);
            sum += prod;
        }
        cache[i] = sum;
    }
    computed = n + 1;
    return cache[n];
}
