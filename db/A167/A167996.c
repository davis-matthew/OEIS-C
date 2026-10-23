/*
 * A167996  Positive numbers n with property that first digit of 3*n = last
 *          digit of n.
 * https://oeis.org/A167996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth positive integer n such that
   firstdigit(3*n) = lastdigit(n).  Computed by incremental
   search and caching of results. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Helper that tests whether a given integer satisfies the
   required property. */
static bool matches(seq_t x)
{
    seq_t prod = x * 3;
    int d = digit_len(prod);                    /* number of decimal digits of 3*x */
    seq_t pow = pow10((idx_t)(d - 1));           /* 10^(d-1) */
    seq_t first = prod / pow;                   /* most significant digit of 3*x */
    seq_t last  = x % 10;                       /* least significant digit of x */
    return first == last;
}

/* Cache for the sequence values. */
static seq_t *cache     = NULL;                /* array of found terms */
static size_t cache_len = 0;                   /* number of elements stored */
static size_t cache_cap = 0;                   /* allocated capacity */
static seq_t current   = 1;                    /* next integer to test */

/* Extends the cache up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find the next integer that matches the property. */
        while (!matches(current)) {
            current++;
        }

        /* Grow the array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Store the found term. */
        cache[cache_len++] = current;
        current++;                  /* advance to next candidate */
    }
}

/* External linkage: returns the nth term of A167996. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; but 0 is never requested */
    ensure_cache(n);
    return cache[n - 1];
}
