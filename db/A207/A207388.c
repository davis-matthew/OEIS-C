/*
 * A207388  Number of n X 5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) +
 *            6*a(n-5) - a(n-6) for n > 6 initial values: a(1) = 14, a(2) =
 *            196, a(3) = 834, a(4) = 2356, a(5) = 5348, a(6) = 10570
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 119, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 187 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *cache = NULL;        /* cache[0] = a(1), cache[1] = a(2), … */
static size_t capacity = 0;        /* allocated size of cache */
static size_t computed = 0;        /* number of values already computed */

/* ensure that the cache can store at least n elements */
static void grow_capacity(idx_t n)
{
    if (capacity < (size_t)n) {
        seq_t *new_cache = seq_alloc_new_array(n);
        if (cache) {
            mem_copy(new_cache, cache, capacity * sizeof(seq_t));
            seq_free_array(cache);
        }
        cache = new_cache;
        capacity = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    if (n > computed) {            /* need to compute more terms */
        grow_capacity(n);

        static const seq_t init[6] = {
            14, 196, 834, 2356, 5348, 10570
        };

        for (size_t i = computed; i < (size_t)n; ++i) {
            seq_t val;
            if (i < 6) {
                val = init[i];
            } else {
                seq_t t1 = mul_small(cache[i - 1], 6);        /* 6*a(n-1) */
                t1 += mul_small(cache[i - 3], 20);           /* +20*a(n-3) */
                t1 += mul_small(cache[i - 5], 6);            /* +6*a(n-5) */

                seq_t t2 = mul_small(cache[i - 2], 15);      /* 15*a(n-2) */
                t2 += mul_small(cache[i - 4], 15);           /* +15*a(n-4) */
                t2 += cache[i - 6];                          /* +a(n-6) */

                val = t1 - t2;                             /* guaranteed non‑negative */
            }
            cache[i] = val;
        }
        computed = (size_t)n;
    }

    return cache[n - 1];
}
