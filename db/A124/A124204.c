/*
 * A124204  Numbers k such that 20*k + 1 is prime.
 * https://oeis.org/A124204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 907 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A124204: a(n) is the n-th integer k such that 20*k+1 is prime
 *  (OEIS offset 1).  The function builds a static cache of found values
 *  so that calls may come in any order.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found k's          */
    static size_t  cap   = 0;     /* allocated capacity         */
    static size_t  size  = 0;     /* number of elements stored  */
    static idx_t   next_k = 1;    /* next candidate k to test   */

    if (n == 0) {
        return (seq_t)0;          /* offset is 1, below not used */
    }

    /* enlarge the cache until it contains at least n elements */
    if (size < (size_t)n)
    {
        size_t need = n;
        if (cap < need) {
            size_t new_cap = cap ? cap << 1 : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = seq_realloc(cache, cap, new_cap);
            if (!tmp) {
                return (seq_t)0;  /* allocation failed */
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* find additional k values until we have n terms */
        while (size < need) {
            idx_t num = (idx_t)20 * next_k + 1;   /* 20*k+1 */
            if (is_prime(num)) {
                cache[size++] = (seq_t)next_k;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
