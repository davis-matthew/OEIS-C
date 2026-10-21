/*
 * A212440  Numbers with even curling numbers of their binary representations,
 *          cf. A181935.
 * https://oeis.org/A212440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th number whose binary representation has an even
 *            curling number (curling number = largest c such that the binary
 *            string ends with c copies of some block)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   curling_number_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/curling_number_uint64.h"

/* Dynamic cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, behaviour for 0 is undefined */
    /* Ensure cache has at least n elements */
    while (cache_len < (size_t)n) {
        uint64_t cand = (uint64_t)(cache_len ? cache[cache_len - 1] : 0) + 1;
        /* Find next number with even curling number */
        while (true) {
            int c = curling_number_uint64(cand);
            if ((c & 1) == 0) {  /* even */
                grow_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
                break;
            }
            cand++;
        }
    }
    return cache[n - 1];
}
