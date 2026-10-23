/*
 * A023435  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-5).
 * https://oeis.org/A023435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) for n >= 5 Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;

static void grow_cache(idx_t n) {
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        seq_t *new_array = alloc_seq_array(new_cap);
        if (!new_array) return; /* allocation failure – caller will receive garbage */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_array[i] = cache[i];
            free_seq(cache);
        }
        cache = new_array;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n <= last_computed && cache) {
        return cache[(size_t)n];
    }
    grow_cache(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)1;
        } else if (i == 3) {
            cache[3] = (seq_t)2;
        } else if (i == 4) {
            cache[4] = (seq_t)3;
        } else {
            cache[i] = cache[i-1] + cache[i-2] - cache[i-5];
        }
    }
    last_computed = n;
    return cache[(size_t)n];
}
