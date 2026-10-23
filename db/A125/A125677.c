/*
 * A125677  a(0) = a(1) = 1; for n>1, a(n) = a(n-2) + a(n-1) (mod n).
 * https://oeis.org/A125677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + a(n-1) (mod n), a(0)=a(1)=1
 * Range:     u8 n <= 45, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cached values of the sequence */
static idx_t cache_size   = 0;      /* allocated size (elements) */
static seq_t *cache       = NULL;   /* the cache array */
static idx_t max_computed = (idx_t)-1;  /* highest n that has been computed;
                                          (idx_t)-1 means none yet */

/* ensure that cache can hold elements up to index n */
static void ensure_cache(idx_t n)
{
    if (n >= cache_size) {
        size_t new_size = (cache_size == 0) ? 4 : cache_size * 2;
        if (new_size <= (size_t)n) new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();          /* out of memory */
        cache      = tmp;
        cache_size = new_size;
    }
}

/* compute a(0)…a(n) and store them in cache */
static void compute_up_to(idx_t n)
{
    if (max_computed == (idx_t)-1) {
        /* initialise the base cases */
        ensure_cache(2);         /* need indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n <= max_computed) return;

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        ensure_cache(i);
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t mod   = prev1 % (seq_t)i;
        cache[i] = prev2 + mod;
    }
    max_computed = n;
}

/* public entry point */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
