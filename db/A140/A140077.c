/*
 * A140077  Numbers n such that n and n+1 have 3 distinct prime factors.
 * https://oeis.org/A140077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9480 (2026-11-03)
 * Helpers:   mem, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the sequence A140077:
 *  numbers n such that n and n+1 each have exactly three distinct prime factors.
 *  The function builds a cache of already found terms; it works for any
 *  build where count_distinct_primes(uint64_t) is provided.
 */
#include <stddef.h>       /* size_t */
#include <stdlib.h>      /* malloc, realloc, free, EXIT_FAILURE */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /*  Cache of found terms.  */
    static seq_t *cache   = NULL;   /* array of seq_t values */
    static size_t size    = 0;      /* number of terms stored  */
    static size_t cap     = 0;      /* allocated capacity      */
    static uint64_t next  = 2;      /* next candidate to test  */

    if (n == 0)                /* OEIS offset is 1; 0 is invalid */
        return 0;

    /*  Grow the cache until we have at least n terms  */
    while (size < (size_t)n) {
        uint64_t i = next;
        /*  Search for the next integer whose two successive values
         *  each have exactly three distinct prime factors.  */
        while (1) {
            if (omega(i) == 3 &&
                omega(i + 1) == 3)
                break;
            ++i;
        }

        /*  Ensure there is room for the new term  */
        if (size + 1 > cap) {
            cap = cap ? cap * 2 : 16;
            /*  realloc returns void*; cast explicitly for C++.  */
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (!tmp) {                  /* allocation failed  */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
        }

        /*  Store the new term and prepare the next search start  */
        cache[size++] = (seq_t)i;
        next = i + 1;                   /* start search after the found term */
    }

    return cache[n - 1];
}
