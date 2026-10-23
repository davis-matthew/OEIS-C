/*
 * A144026  An INVERT transform of the Thue-Morse sequence.
 * https://oeis.org/A144026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} A010060(n-k) * a(k), a(0)=1 A010060(n) is
 *            the Thue–Moore value: 0 if popcount(n) is even, 1 otherwise
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache of already computed a(n) values */
static seq_t *cache = NULL;      /* a(0)..a(max_computed) */
static size_t cache_len = 0;     /* number of elements stored (>=1 after init) */

/* ensure the cache holds values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len) return;
    size_t new_len = (size_t)(n + 1);
    seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!p) exit(EXIT_FAILURE);     /* out of memory */
    cache = p;
    cache_len = new_len;
}

/* Thue–Moore value: 0 if parity of popcount(n) is even, 1 otherwise */
static inline int thue_morse(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_parityll((unsigned long long)n);
#else
    unsigned long long v = (unsigned long long)n;
    v ^= v >> 32;
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return v & 1;
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise a(0) if necessary */
    if (cache_len == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if ((idx_t)cache_len > n)            /* already computed */
        return cache[(size_t)n];

    size_t old_len = cache_len;          /* last computed index + 1 */
    ensure_cache(n);                     /* make space for a[0..n] */

    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= i; ++j) {
            if (thue_morse(j))            /* A010060(j) == 1 when parity odd */
                sum += cache[(size_t)(i - j)];
        }
        cache[(size_t)i] = sum;
    }
    return cache[(size_t)n];
}
