/*
 * A043734  Numbers whose number of runs in the base-2 representation is
 *          congruent to 1 mod 6.
 * https://oeis.org/A043734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Generate the sequence by scanning integers and selecting
 * those whose binary representation has a number of runs congruent to 1 mod 6. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/*  static cache for memoization across calls  */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static seq_t last_value = 0;

/*  Count runs in the binary representation of x  */
static int runs_in_binary(seq_t x)
{
    if (x == 0) return 0;
    int runs = 1;
    seq_t prev = x & 1;
    x >>= 1;
    while (x != 0) {
        seq_t bit = x & 1;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        x >>= 1;
    }
    return runs;
}

/*  OEIS A043734  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, return 0 as safety */
    /* allocate or resize cache if needed */
    if (cache == NULL) {
        cache = seq_alloc_new_array((size_t)n);
        cache_size = 0;
    } else if ((size_t)n > cache_size && cache != NULL) {
        seq_t *new_cache = seq_alloc_new_array((size_t)n);
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
    }

    /* generate missing terms up to n */
    seq_t x = last_value + 1;
    while (cache_size < (size_t)n) {
        if (runs_in_binary(x) % 6 == 1) {
            cache[cache_size++] = x;
            last_value = x;
        }
        ++x;
    }
    return cache[(size_t)n - 1];
}
