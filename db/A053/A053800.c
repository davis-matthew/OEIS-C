/*
 * A053800  Number of basis partitions of n+25 with Durfee square size 5.
 * https://oeis.org/A053800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 4*a(n-5) * -
 *            4*a(n-6) + 3*a(n-7) - 2*a(n-8) + 3*a(n-9) - 3*a(n-10) + a(n-11)
 *            * for n > 11 * a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) =
 *            14, a(5) = 24, * a(6) = 38, a(7) = 58, a(8) = 86, a(9) = 124,
 *            a(10) = 174, a(11) = 238 * This function comp...
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 788, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached terms */
static size_t cache_size = 0; /* number of cached terms */

static void init_cache(void) {
    if (cache != NULL) return;   /* already initialized */

    /* Initial terms up to a(11) */
    const seq_t init_values[12] = {
        (seq_t)1,  /* a(0) */
        (seq_t)2,  /* a(1) */
        (seq_t)4,  /* a(2) */
        (seq_t)8,  /* a(3) */
        (seq_t)14, /* a(4) */
        (seq_t)24, /* a(5) */
        (seq_t)38, /* a(6) */
        (seq_t)58, /* a(7) */
        (seq_t)86, /* a(8) */
        (seq_t)124,/* a(9) */
        (seq_t)174,/* a(10) */
        (seq_t)238 /* a(11) */
    };

    /* Allocate at least 12 entries, with some spare room for growth */
    size_t initial_capacity = 32;
    cache = (seq_t *)malloc(initial_capacity * sizeof(seq_t));
    for (size_t i = 0; i < 12; ++i) cache[i] = init_values[i];
    for (size_t i = 12; i < initial_capacity; ++i) cache[i] = 0;

    cache_size = initial_capacity;
}

static void grow_cache_to(size_t needed) {
    if (needed < cache_size) return;
    size_t new_size = cache_size * 2;
    if (new_size < needed) new_size = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) { /* allocation failed; abort (no error handling requested) */
        abort();
    }
    cache = new_cache;
    for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    /* Ensure cache is ready */
    init_cache();

    if ((size_t)n < 12) {
        /* Directly return one of the initial values */
        return cache[(size_t)n];
    }

    /* Compute values up to n lazily, expanding cache as needed */
    size_t upto = (size_t)n;
    if (upto >= cache_size) {
        grow_cache_to(upto + 1);
    }

    /* Compute from first missing index onward */
    for (size_t i = 12; i <= upto; ++i) {
        seq_t val = 0;
        val += (3 * cache[i-1]);
        val += (-3 * cache[i-2]);
        val += (2 * cache[i-3]);
        val += (-3 * cache[i-4]);
        val += (4 * cache[i-5]);
        val += (-4 * cache[i-6]);
        val += (3 * cache[i-7]);
        val += (-2 * cache[i-8]);
        val += (3 * cache[i-9]);
        val += (-3 * cache[i-10]);
        val += (1 * cache[i-11]);
        cache[i] = val;
    }

    return cache[upto];
}
