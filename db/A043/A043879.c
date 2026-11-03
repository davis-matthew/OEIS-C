/*
 * A043879  Numbers k such that 1 and 2 occur juxtaposed in the base-3
 *          representation of k but not of k+1.
 * https://oeis.org/A043879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search.  a(n) is the nth integer k such that
   the base‑3 representation of k contains an adjacent pair 12 or 21
   (i.e. a “12” or “21” substring) and the base‑3 representation of k+1
   does not contain such a pair. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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

/* Check whether x has adjacent digits 12 or 21 in base‑3. */
static bool has_adj12_21(idx_t x)
{
    int last = -1;
    while (x > 0) {
        int d = x % 3;
        if (last != -1) {
            if ((last == 1 && d == 2) || (last == 2 && d == 1))
                return true;
        }
        last = d;
        x /= 3;
    }
    return false;
}

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, harmless return */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next integer that meets the definition. */
        while (true) {
            if (has_adj12_21(next_candidate) &&
                !has_adj12_21(next_candidate + 1))
                break;
            ++next_candidate;
        }

        /* Ensure sufficient capacity for the new term. */
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = seq_alloc(cache_cap);
        } else if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
