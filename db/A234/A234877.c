/*
 * A234877  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 99,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + 11*a(n-2) - 23*a(n-3)
 *             - 38*a(n-4) + 87*a(n-5) + 46*a(n-6)
 *             - 130*a(n-7) - 16*a(n-8) + 78*a(n-9)
 *             - 16*a(n-11)            (n >= 12)
 * Initial terms (n = 1 .. 11):
 * 128, 268, 472, 1116, 2096, 5316, 10240, 27060, 52744, 142876, 280240
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static seq_t *cache = NULL;          /* Array of computed terms, 1‑based */
static idx_t  cache_len = 0;         /* Highest index that has been computed */
static idx_t  cache_cap = 0;         /* Current capacity of the array */

/* Helper: ensure the cache can hold up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial allocation: enough for the 11 known terms */
        cache_cap = 16;
        if (cache_cap < 12) cache_cap = 12;
        cache = seq_alloc_new_array(cache_cap);
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        /* Fill in the known initial terms */
        cache[1]  = (seq_t)128;
        cache[2]  = (seq_t)268;
        cache[3]  = (seq_t)472;
        cache[4]  = (seq_t)1116;
        cache[5]  = (seq_t)2096;
        cache[6]  = (seq_t)5316;
        cache[7]  = (seq_t)10240;
        cache[8]  = (seq_t)27060;
        cache[9]  = (seq_t)52744;
        cache[10] = (seq_t)142876;
        cache[11] = (seq_t)280240;
        cache_len = 11;
    }

    if (n <= cache_len) return;

    /* Grow the cache to at least n */
    while (cache_cap <= n) cache_cap <<= 1;
    seq_t *new_cache = seq_alloc_new_array(cache_cap);
    /* Copy existing data */
    for (idx_t i = 0; i <= cache_len; ++i) new_cache[i] = cache[i];
    seq_free(cache);
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Skip if within the known initial range */
        if (k <= 11) continue;

        seq_t pos = 0;
        pos += 2      * cache[k-1];
        pos += 11     * cache[k-2];
        pos += 87     * cache[k-5];
        pos += 46     * cache[k-6];
        pos += 78     * cache[k-9];

        seq_t neg = 0;
        neg += 23     * cache[k-3];
        neg += 38     * cache[k-4];
        neg += 130    * cache[k-7];
        neg += 16     * cache[k-8];
        neg += 16     * cache[k-11];

        cache[k] = pos - neg;      /* pos >= neg for all valid n */
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n < 1, but return 0 */
    ensure_cache(n);
    return cache[n];
}
