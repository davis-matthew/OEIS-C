/*
 * A023615  Convolution of Fibonacci numbers and primes.
 * https://oeis.org/A023615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + prime(n+1), a(0)=2, a(1)=5
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/ensure_cache.h"  /* ensure_cache()   */
#include "../utils/get_prime.h"     /* get_prime()      */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t computed   = 0;   /* highest index stored (>=1 after init) */

    /* initialise cache for first two values */
    if (cache == NULL) {
        ensure_cache((idx_t)1, &cache, &cache_cap);  /* indices 0 .. 1 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        computed = 1;
    }

    /* fast return if already known */
    if (n <= computed)
        return cache[n];

    /* grow cache as needed */
    ensure_cache(n, &cache, &cache_cap);

    /* compute missing values using recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] +
                   (seq_t)get_prime((idx_t)i + 1);
    }

    computed = n;
    return cache[n];
}
