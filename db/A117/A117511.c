/*
 * A117511  Triangular numbers for which the sum of the digits equals the sum
 *          of the digits of the next triangular number.
 * https://oeis.org/A117511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th triangular number T_k such that s(T_k) =
 *            s(T_{k+1})
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 900 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                                                   */
/* This implementation builds the sequence incrementally. It uses the    */
/* provided digit_sum function to compare digit sums and stores found */
/* values in a dynamically resizable array. The array is extended as   */
/* necessary to satisfy future requests, so any order of calls is       */
/* supported.                                                           */

#include <stddef.h>              /* size_t */
#include <stdbool.h>             /* bool */

/* Utility functions for dynamic allocation of seq_t arrays */
#include <stdlib.h>
#include "../utils/digit_sum.h"

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

/* Function that returns the sum of the decimal digits of a seq_t. */

static seq_t *cache     = NULL;   /* array of found terms */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t cache_len  = 0;       /* number of terms stored */
static idx_t last_k     = 0;       /* largest k processed so far */

/* Ensure that the array contains at least n_needed terms. */
static void
ensure_terms(idx_t n_needed)
{
    /* Grow the array if necessary. */
    if (cache_cap < (size_t)n_needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n_needed) new_cap <<= 1;
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate triangular numbers until we have enough terms. */
    idx_t k = last_k + 1;          /* start from the next k */
    while (cache_len < n_needed) {
        seq_t Tk  = (seq_t)k * ((seq_t)k + 1) / 2;
        seq_t Tk1 = (seq_t)(k + 1) * ((seq_t)(k + 1) + 1) / 2;
        if (digit_sum(Tk) == digit_sum(Tk1)) {
            cache[cache_len++] = Tk;
        }
        ++k;
    }
    last_k = k - 1;                  /* last processed k */
}

/* Public sequence function.  */
seq_t
a(idx_t n)
{
    if (n == 0) {                   /* undefined; return 0 for safety */
        return (seq_t)0;
    }
    ensure_terms(n);
    return cache[n - 1];
}
