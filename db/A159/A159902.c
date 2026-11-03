/*
 * A159902  Concatenation of the first n terms of A144338.
 * https://oeis.org/A159902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 334 (2026-11-03)
 * Helpers:   is_squarefree, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of squarefree numbers A005117(2) to A005117(n+1). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition, but guard against 0 */
    if (n == 0) return (seq_t)0;

    /* Static cache of previously computed prefix concatenations. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  candidate = 2;   /* next integer to test for squarefreeness */

    /* Allocate cache on first use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next squarefree number. */
        while (!is_squarefree((seq_t)candidate)) {
            candidate++;
        }

        seq_t term     = (seq_t)candidate;
        seq_t new_val;

        if (cache_len == 0) {
            /* The very first term is just the number itself. */
            new_val = term;
        } else {
            /* Append the new term to the previous concatenation. */
            seq_t mult = cache[cache_len - 1] * pow10((idx_t)num_digits_idx(candidate));
            new_val = mult + term;
        }

        /* Ensure the cache has room for another element. */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;  /* double the capacity */
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        cache[cache_len++] = new_val;
        candidate++;         /* advance to the next integer for the next iteration */
    }

    return cache[n - 1];
}
