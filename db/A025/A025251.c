/*
 * A025251  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 0, 1, 2, 0.
 * https://oeis.org/A025251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + … + a(n-3)*a(3), with a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=0. It is memoized and works regardless of
 *            the order of calls
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Static cache – 1‑based indexing; cache[0] is unused. */
static seq_t *cache = NULL;
static size_t   cap      = 0;             /* allocated size of cache   */
static idx_t    max_comp = 0;             /* largest n already computed */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0 … n          */
    if (needed <= cap) return;

    size_t new_cap = cap ? cap << 1 : 1;     /* grow exponentially   */
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return;                       /* allocation failure – caller will see stale values */

    if (cache) {
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        free(cache);
    }

    cache = tmp;
    cap   = new_cap;
}

/* Computes the n‑th term of A025251. */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;             /* undefined for n < 1 */

    /* First initialisation on first call. */
    if (!cache) {
        ensure_cache(5);                     /* at least indices 0…4 */
        cache[0]      = (seq_t)0;            /* unused */
        cache[1]      = (seq_t)0;            /* a(1) */
        cache[2]      = (seq_t)1;            /* a(2) */
        cache[3]      = (seq_t)2;            /* a(3) */
        cache[4]      = (seq_t)0;            /* a(4) */
        max_comp = 4;
    }

    if (n <= max_comp) return cache[n];     /* already computed */

    ensure_cache(n);

    /* Compute missing terms in increasing order. */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        if (k < 5) {                        /* safety for k = 5… */
            cache[k] = (seq_t)0;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= k - 3; ++i) {
            sum += cache[i] * cache[k - i];
        }
        cache[k] = sum;
    }

    max_comp = n;
    return cache[n];
}
