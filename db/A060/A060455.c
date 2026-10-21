/*
 * A060455  7th-order Fibonacci numbers with a(0)=...=a(6)=1.
 * https://oeis.org/A060455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6) +
 *            a(n-7), a(0) = a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* memoisation array */
static idx_t cache_size = 0;  /* number of terms stored (up to index cache_size-1) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Resize array if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first seven terms if this is the first call */
    if (cache_size == 0) {
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = (seq_t)1;
        cache_size = 7;          /* we have terms 0..6 available */
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 7; ++k)
            sum += cache[i - k];
        cache[i] = sum;
    }
    if (n + 1 > cache_size)
        cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 7)                  /* initial terms are all 1 */
        return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
