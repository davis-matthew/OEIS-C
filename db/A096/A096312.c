/*
 * A096312  Numbers n such that number of prime divisors (counting
 *          multiplicity) = largest digit.
 * https://oeis.org/A096312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   bigomega, maxdigit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A096312: numbers whose total number of prime factors
   (with multiplicity) equals their largest decimal digit.  The
   sequence is generated lazily and cached, so calls with arbitrary
   order of indices are supported. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/maxdigit.h"

/* Helper: grow a dynamic array if needed. */
static inline void ensure_cache_cap(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed > *cap) {
        size_t new_cap = *cap ? (*cap << 1) : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* Out‑of‑memory */
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* capacity of the array */
    static idx_t last_checked = 0;/* last integer examined */

    /* Populate cache until it contains at least n terms. */
    while (len < n) {
        last_checked++;                         /* next integer to test */

        if (maxdigit(last_checked) == (idx_t)bigomega(last_checked)) {
            /* Ensure space for the new term. */
            ensure_cache_cap(len + 1, &cap, &cache);

            /* Store the term as seq_t. */
            cache[len++] = (seq_t)last_checked;
        }
    }

    /* Indexing is 1‑based: a(1) is the first stored value. */
    return cache[n - 1];
}
