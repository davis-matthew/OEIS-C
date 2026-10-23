/*
 * A025238  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-2)*a(2) for n >= 3.
 * https://oeis.org/A025238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-2)*a(2) for n >= 3
 *            with a(1)=3, a(2)=1. Recursive linear form is used: n*a(n) +
 *            3*(-2n+3)*a(n-1) + 5*(n-3)*a(n-2) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 0‑based array: cache[0]=a(1) */
static idx_t cache_size = 0;          /* number of entries already in cache */
static size_t cache_capacity = 0;     /* allocated capacity */

static void ensure_cache(idx_t n) {
    if (n <= cache_capacity)
        return;
    size_t new_cap = (cache_capacity == 0) ? 1 : cache_capacity;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) /* undefined below offset, return 0 for safety */
        return 0;
    ensure_cache(n);
    if (n <= cache_size)
        return cache[n - 1];

    /* compute missing entries */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k == 1) {
            cache[0] = 3;
        } else if (k == 2) {
            cache[1] = 1;
        } else {
            /* a(k) = (3*(2k-3)*a(k-1) - 5*(k-3)*a(k-2)) / k */
            seq_t t1 = 3 * (seq_t)(2 * k - 3) * cache[k - 2];   /* a(k-1) */
            seq_t t2 = 5 * (seq_t)(k - 3) * cache[k - 3];       /* a(k-2) */
            cache[k - 1] = (t1 - t2) / (seq_t)k;
        }
    }
    cache_size = n;
    return cache[n - 1];
}
