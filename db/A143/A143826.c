/*
 * A143826  Numbers k such that 6*k^2 - 1 is prime.
 * https://oeis.org/A143826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that 6*k^2-1 is prime
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;       /* stored k values */
static size_t cache_cap = 0;      /* allocated size of cache */
static size_t cache_size = 0;     /* number of filled entries */
static idx_t tested_k = 0;        /* largest k that has been tested */

/* grow the cache until it can hold at least 'need' entries */
static void ensure_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* return the n-th term of A143826 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)                     /* below offset – behaviour irrelevant */
        return 0;

    if ((size_t)n <= cache_size)    /* already computed */
        return cache[n - 1];

    /* search forward until we have n values */
    while (cache_size < (size_t)n) {
        ++tested_k;
        seq_t kk = (seq_t)tested_k;
        seq_t val = 6 * kk * kk - 1;  /* 6*k^2-1 */
        if (is_prime_seq(val)) {    /* primality test */
            ensure_cap(cache_size + 1);
            cache[cache_size++] = tested_k;
        }
    }

    return cache[n - 1];
}
