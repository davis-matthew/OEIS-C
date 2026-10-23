/*
 * A039602  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 10's.
 * https://oeis.org/A039602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039602: Numbers whose base‑12 representation has the same
   nonzero number of 0's and 10's.  Computed by brute‑force search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values.  The cache grows as needed. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t next_test   = 0;

/* Test whether x satisfies the property. */
static bool satisfies(seq_t x)
{
    seq_t t   = x;
    unsigned c0  = 0;
    unsigned c10 = 0;
    while (t) {
        seq_t d = t % (seq_t)12;
        if (d == 0)
            ++c0;
        else if (d == 10)
            ++c10;
        t /= (seq_t)12;
    }
    return c0 == c10 && c0 > 0;
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;                    /* undefined for n=0, harmless */

    while (cache_size < (size_t)n) {
        if (satisfies(next_test)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                     new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Allocation failed */
                    abort();
                }
                cache     = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_test;
        }
        ++next_test;
    }
    return cache[n - 1];
}
