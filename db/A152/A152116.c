/*
 * A152116  Sixth degree product sequence: a(n) = Product_{k=1..(n-1)/2} (1 +
 *          4*cos(k*Pi/n)^2 + 16*cos(k*Pi/n)^4 + 64*cos(k*Pi/n)^6).
 * https://oeis.org/A152116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..(n-1)/2} (1 + 4*cos(k*Pi/n)^2 +
 *            16*cos(k*Pi/n)^4 + 64*cos(k*Pi/n)^6)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n >= 9
 * a(n) = a(n-1) + 7*a(n-2) + 6*a(n-3)
 *        - 8*a(n-4) - 6*a(n-5) + 7*a(n-6)
 *        - a(n-7) - a(n-8)
 */
#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static const seq_t base[9] = {
    (seq_t)1,  // a(0)
    (seq_t)1,  // a(1)
    (seq_t)1,  // a(2)
    (seq_t)4,  // a(3)
    (seq_t)15, // a(4)
    (seq_t)45, // a(5)
    (seq_t)160,// a(6)
    (seq_t)533,// a(7)
    (seq_t)1785// a(8)
};

seq_t a(idx_t n)
{
    /* Allocate or extend static cache if necessary */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < 9)
        return base[n];

    /* Ensure cache can hold up to n */
    if (cache_len <= n) {
        /* Allocate new cache big enough */
        seq_t *new_cache = seq_alloc_array((size_t)(n + 1));
        /* Copy existing data if any */
        if (cache) {
            size_t copy_len = (size_t)cache_len;
            for (size_t i = 0; i < copy_len; ++i)
                new_cache[i] = cache[i];
        }
        /* Initialize first 9 terms if not already set */
        for (size_t i = cache_len; i < 9 && i <= n; ++i)
            new_cache[i] = base[i];
        /* Compute new terms using recurrence */
        for (size_t i = cache_len; i <= n; ++i) {
            if (i < 9) continue;          /* already initialized */
            seq_t val = new_cache[i-1];
            val += mul_small(new_cache[i-2], 7u);
            val += mul_small(new_cache[i-3], 6u);
            val -= mul_small(new_cache[i-4], 8u);
            val -= mul_small(new_cache[i-5], 6u);
            val += mul_small(new_cache[i-6], 7u);
            val -= new_cache[i-7];
            val -= new_cache[i-8];
            new_cache[i] = val;
        }
        /* Free old cache if it existed */
        if (cache)
            seq_free_array(cache);
        cache     = new_cache;
        cache_len = n + 1;
    }

    return cache[n];
}
