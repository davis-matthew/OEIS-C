/*
 * A125316  Number of base 6 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 8*a(n-3) + a(n-4) + 2*a(n-5) for
 *            n>5 Initial values: a(0)=1, a(1)=6, a(2)=30, a(3)=138, a(4)=694,
 *            a(5)=3526
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* array that stores computed terms */
static idx_t  cache_len = 0;     /* number of terms stored (size of cache) */
static size_t cache_cap = 0;     /* current capacity in seq_t elements */

/* Ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)          /* already computed */
        return;

    /* Increase capacity if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(1);           /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 0: val = (seq_t)1; break;
            case 1: val = (seq_t)6; break;
            case 2: val = (seq_t)30; break;
            case 3: val = (seq_t)138; break;
            case 4: val = (seq_t)694; break;
            case 5: val = (seq_t)3526; break;
            default: {
                seq_t t = (seq_t)6 * cache[i - 1];
                t -= (seq_t)3 * cache[i - 2];
                t -= (seq_t)8 * cache[i - 3];
                t += cache[i - 4];
                t += (seq_t)2 * cache[i - 5];
                val = t;
            }
        }
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
