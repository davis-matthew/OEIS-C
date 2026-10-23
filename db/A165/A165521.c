/*
 * A165521  The number of 4321-avoiding separable permutations of length n.
 * https://oeis.org/A165521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 19*a(n-2) + 28*a(n-3) - 23*a(n-4) + 12*a(n-5)
 *            - 4*a(n-6) + a(n-7), for n ≥ 7 Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=243
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   allocate_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;     /* array of computed terms          */
static size_t cache_cap = 0;    /* allocated capacity of cache      */
static idx_t cache_len = 0;     /* number of terms currently stored */

static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;            /* already computed up to n   */

    size_t needed = (size_t)n + 1;         /* minimal capacity needed   */

    /* Allocate or grow the cache array */
    if (!cache) {
        cache_cap = (needed > 64 ? needed : 64);
        cache = allocate_seq_array(cache_cap);
        cache_len = 0;
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Initialise the first 7 terms if they are not yet set. */
    if (cache_len < 7) {
        const seq_t init[7] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
                                (seq_t)21, (seq_t)73, (seq_t)243 };
        for (idx_t i = cache_len; i < 7; ++i)
            cache[i] = init[i];
        cache_len = 7;
    }

    /* Compute further terms up to index n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)7;
        val -= cache[i-2] * (seq_t)19;
        val += cache[i-3] * (seq_t)28;
        val -= cache[i-4] * (seq_t)23;
        val += cache[i-5] * (seq_t)12;
        val -= cache[i-6] * (seq_t)4;
        val += cache[i-7];
        cache[i] = val;
    }
    cache_len = n + 1;                      /* now cache contains 0..n   */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
