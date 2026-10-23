/*
 * A021614  Expansion of 1/((1-x)*(1-3*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A021614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 152*a(n-2) + 362*a(n-3) - 231*a(n-4), a(0) =
 *            1, a(1) = 22, a(2) = 332, a(3) = 4322
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values */
static size_t cache_size = 0; /* number of valid entries */
static size_t cache_cap   = 0; /* allocated capacity */

static void cache_grow(size_t new_cap) {
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; terminate the program safely */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

static void cache_prepare(void) {
    if (cache_size == 0) {
        cache_grow(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)332;
        cache[3] = (seq_t)4322;
        cache_size = 4;
    }
}

seq_t a(idx_t n) {
    /* Ensure cache is initialized */
    cache_prepare();

    size_t index = (size_t)n;
    if (index < cache_size) {
        return cache[index];
    }

    /* Compute values up to the requested index */
    while (cache_size <= index) {
        seq_t a0 = cache[cache_size - 4];
        seq_t a1 = cache[cache_size - 3];
        seq_t a2 = cache[cache_size - 2];
        seq_t a3 = cache[cache_size - 1];

        seq_t value = (seq_t)22 * a3
                    - (seq_t)152 * a2
                    + (seq_t)362 * a1
                    - (seq_t)231 * a0;

        if (cache_size == cache_cap) {
            cache_grow(cache_cap ? cache_cap * 2 : 4);
        }
        cache[cache_size] = value;
        ++cache_size;
    }

    return cache[index];
}
