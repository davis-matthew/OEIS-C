/*
 * A072987  FIBMOD numbers: a(1) = a(2) = 1, a(n) = a(n-1) mod (n-1) + a(n-2)
 *          mod (n-2).
 * https://oeis.org/A072987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 1, a(n) = a(n-1) mod (n-1) + a(n-2) mod (n-2)
 *            (n ≥ 3)
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0..size-1] holds terms for indices 0..size-1 */
static idx_t  cache_size   = 0;       /* current allocated size (number of elements) */
static idx_t  last_computed = 0;      /* largest n for which a(n) is known */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;            /* already enough */
    idx_t new_size = cache_size ? cache_size * 2 : 1;
    if (new_size <= n) new_size = n + 1;    /* need at least n+1 slots (0..n) */

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!tmp) {                            /* allocation failure: abort */
        exit(1);
    }
    cache      = tmp;
    cache_size = new_size;
}

/* Compute values up to n (inclusive).  Cache is guaranteed large enough. */
static void compute_up_to(idx_t n)
{
    while (last_computed < n) {
        ++last_computed;
        if (last_computed == 1 || last_computed == 2) {
            cache[last_computed] = (seq_t)1;
            continue;
        }
        seq_t m1 = cache[last_computed - 1] % (seq_t)(last_computed - 1);
        seq_t m2 = cache[last_computed - 2] % (seq_t)(last_computed - 2);
        cache[last_computed] = m1 + m2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined for n=0; return 0 */

    ensure_cache(n);
    compute_up_to(n);
    return cache[n];
}
