/*
 * A043690  a(n) = (s(n)-1)/2, where s(n) is the n-th number whose base-2
 *          representation has exactly 9 runs.
 * https://oeis.org/A043690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n) - 1) / 2, where * s(n) is the nth integer whose
 *            binary representation has exactly 9 runs
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   runs_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../utils/runs_of_uint64.h"

#ifndef __cplusplus
/* In C, cast from void* to pointer type is allowed. */
#define CAST_PTR(ptr, type) ((type*)(ptr))
#else
/* In C++, use reinterpret_cast for safety. */
#define CAST_PTR(ptr, type) (reinterpret_cast<type*>(ptr))
#endif

/* Static cache for already computed terms. */
static seq_t  *cache = NULL;   /* array of computed a(k) values */
static size_t  cached  = 0;    /* number of values stored */
static size_t  cap     = 0;    /* current allocation capacity */
static idx_t   next_num = 1;   /* next candidate integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range */

    /* Expand the cache until it holds the requested term. */
    while (cached < (size_t)n) {
        /* Find the next integer with exactly nine runs. */
        for (;;) {
            ++next_num;
            if (runs_of_uint64((uint64_t)next_num) == 9)
                break;
        }

        /* Compute the sequence value. */
        seq_t val = ((seq_t)next_num - (seq_t)1) / (seq_t)2;

        /* Grow the array if necessary. */
        if (cached == cap) {
            size_t new_cap = (cap ? cap << 1 : 1024);
            seq_t *tmp = CAST_PTR(realloc(cache, new_cap * sizeof(seq_t)), seq_t);
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        cache[cached++] = val;
    }

    return cache[n - 1];
}
