/*
 * A163551  13th-order Fibonacci numbers: a(n) = a(n-1) + ... + a(n-13) with
 *          a(1)=...=a(13)=1.
 * https://oeis.org/A163551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+...+a(n-13), a(1)=...=a(13)=1
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 42, u64 n <= 74, u128 n <= 138,
 *            big any n
 * Helpers:   alloc_seq_buf, free_seq_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset: 1 */

#include <stdlib.h>
#include "../utils/alloc_seq_buf.h"
#include "../utils/free_seq_buf.h"

static seq_t *cache = NULL;          /* static memoisation buffer */
static size_t cache_cap = 0;         /* current buffer capacity (in terms) */
static size_t cache_len = 0;         /* number of terms already stored */

/* Helper to deallocate a buffer of seq_t usable from both C and C++ */

/* Ensure the cache can hold at least target terms */
static void grow_cache(idx_t target) {
    if ((size_t)target <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)target) new_cap *= 2;
    seq_t *new_cache = alloc_seq_buf(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free_seq_buf(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* undefined for offset < 1, safe to return 0 */

    /* Resize cache if required */
    grow_cache(n);

    /* Initialise the first 13 terms if not already done */
    if (cache_len < 13) {
        for (idx_t i = cache_len + 1; i <= 13; ++i) {
            cache[(size_t)(i - 1)] = (seq_t)1;
        }
        cache_len = 13;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 13; ++j) {
            sum += cache[(size_t)(i - j - 1)];
        }
        cache[(size_t)(i - 1)] = sum;
    }

    cache_len = n;
    return cache[(size_t)(n - 1)];
}
