/*
 * A044512  Numbers n such that string 4,5 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   contains45_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044512 – Numbers n such that the base‑6 representation of n contains the
   substring “45” while the base‑6 representation of n+1 does not.
   The algorithm enumerates integers in increasing order, keeping a
   static array of all found numbers.  When the requested index n is
   larger than the stored length, it continues enumerating until the
   array contains at least n elements.
   This method runs in O(n * log_6 M) time, where M is the
   highest number examined; for n up to 10 000 M is below 400 000,
   which is comfortably fast in C11/C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains45_base6.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* grow the cache as needed */
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* find the next integer whose base‑6 representation contains 45
           but the next integer does not */
        while (!(contains45_base6(next_candidate) &&
                 !contains45_base6(next_candidate + (seq_t)1))) {
            next_candidate++;
        }
        cache[cache_size++] = next_candidate;
        next_candidate++;
    }

    return cache[(size_t)n - 1];
}
