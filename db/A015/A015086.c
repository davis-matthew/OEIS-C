/*
 * A015086  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=5.
 * https://oeis.org/A015086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = Σ_{i=0}^{n-1} 5^i * a(i) * a(n-1-i), a(0) = 1 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;   /* cache[0..cache_cap-1] */
static idx_t  cache_cap   = 0;     /* number of allocated entries */
static idx_t  cache_n     = 0;     /* highest index computed */

static seq_t *pow5        = NULL;   /* pow5[e] = 5^e  */
static idx_t  pow5_cap    = 0;     /* number of allocated entries */

static void pow5_ensure(idx_t max)
{
    if (pow5_cap > max) return;
    idx_t new_cap = pow5_cap ? pow5_cap : 1;
    while (new_cap <= max) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(pow5, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    pow5 = tmp;

    idx_t start = pow5_cap;
    if (pow5_cap == 0) {
        pow5[0] = (seq_t)1;
        start   = 1;
    }
    for (idx_t e = start; e < new_cap; ++e)
        pow5[e] = pow5[e-1] * (seq_t)5;

    pow5_cap = new_cap;
}

static void cache_ensure(idx_t needed)
{
    if (cache_cap > needed) return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_cap = 1;
        cache_n   = 0;
    }
    if (pow5 == NULL) {
        pow5 = (seq_t*)malloc(sizeof(seq_t));
        if (!pow5) abort();
        pow5[0] = (seq_t)1;
        pow5_cap = 1;
    }

    if (n <= cache_n)
        return cache[n];

    if (n > 0)
        pow5_ensure(n-1);

    cache_ensure(n);

    for (idx_t k = cache_n + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < k; ++i) {
            seq_t prod = cache[i] * cache[k-1-i];
            prod *= pow5[i];
            sum += prod;
        }
        cache[k] = sum;
    }
    cache_n = n;
    return cache[n];
}
