/*
 * A043808  Numbers n such that number of runs in base 3 representation of n
 *          is congruent to 2 mod 9.
 * https://oeis.org/A043808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043808: numbers n whose base‑3 representation has a number
   of runs congruent to 2 (mod 9).  Returns the n‑th such positive integer
   (a(1) = 3).  The implementation uses a simple incremental generator
   and a static cache to support random access. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache for generated terms */
static seq_t *cache = NULL;      /* array of found terms      */
static idx_t  cache_len = 0;     /* number of terms stored    */
static idx_t  cache_cap = 0;     /* allocated array capacity  */

static seq_t next_candidate = (seq_t)1;  /* next integer to test               */

/* Ensure cache has at least min_cap entries */
static void grow_cache(idx_t min_cap)
{
    if (cache_cap < min_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* External linkage function requested by the build system */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n entries */
    while (cache_len < n) {
        seq_t temp = next_candidate;
        int runs = 0;
        int prev = -1; /* sentinel for "no previous digit" */

        while (temp > (seq_t)0) {
            int digit = (int)(temp % (seq_t)3);
            if (digit != prev) {
                ++runs;
                prev = digit;
            }
            temp = temp / (seq_t)3;
        }

        if (runs % 9 == 2) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
