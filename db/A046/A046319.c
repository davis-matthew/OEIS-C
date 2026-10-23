/*
 * A046319  Odd numbers divisible by exactly 6 primes (counted with
 *          multiplicity).
 * https://oeis.org/A046319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes odd numbers with exactly 6 prime factors (counted with multiplicity) */
#include <stdlib.h>
#include "../utils/bigomega.h"

/* Return the n-th term of A046319 (offset 1). */
seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t   processed = 1;  /* next integer to test */

    /* a(0) is unused – return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* guarantee enough space for the cache */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* determine the next block of candidates to test */
        idx_t limit;
        if (processed < 1000)
            limit = 1000;
        else {
            /* double the range, but guard against overflow */
            limit = processed;
            if (limit <= UINT64_MAX / 2)
                limit = limit * 2;
            else
                limit = UINT64_MAX;
        }

        for (idx_t num = processed; num <= limit; ++num) {
            /* skip even numbers */
            if ((num & 1U) == 0) continue;

            /* if the number has exactly 6 prime factors */
            if (bigomega(num) == (seq_t)6) {
                if (cache_len == cache_cap) {
                    /* grow the cache */
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (tmp == NULL) return (seq_t)0;  /* realloc failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)num;
            }
        }

        processed = limit + 1;  /* next number to start from */
    }

    return cache[n - 1];  /* 1‑based indexing */
}
