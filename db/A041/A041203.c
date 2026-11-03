/*
 * A041203  Denominators of continued fraction convergents to sqrt(112).
 * https://oeis.org/A041203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 254*a(n-6) - a(n-12) (n >= 12) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=12, a(6)=247, a(7)=259,
 *            a(8)=506, a(9)=1271, a(10)=1777, a(11)=3048
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 197 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    /* Handle the first 12 terms directly */
    if (n < 12) {
        static const seq_t init[12] = {
            (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)7,   (seq_t)12,
            (seq_t)247,(seq_t)259,(seq_t)506,(seq_t)1271,(seq_t)1777,(seq_t)3048
        };
        return init[(size_t)n];
    }

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;  /* capacity in elements */
    static idx_t computed = 0;         /* number of entries actually computed */

    /* Ensure the cache can hold up to index n */
    ensure_cache(n + 1, &cache, &cache_capacity);

    /* If we haven't yet filled the initial 12 entries, do so */
    if (computed < 12) {
        static const seq_t init[12] = {
            (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)7,   (seq_t)12,
            (seq_t)247,(seq_t)259,(seq_t)506,(seq_t)1271,(seq_t)1777,(seq_t)3048
        };
        for (idx_t i = computed; i < 12; ++i) {
            cache[i] = init[i];
        }
        computed = 12;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = (seq_t)((seq_t)254 * cache[i - 6] - cache[i - 12]);
    }
    computed = n + 1;

    return cache[n];
}
