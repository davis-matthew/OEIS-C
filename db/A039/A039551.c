/*
 * A039551  Numbers whose base-12 representation has the same nonzero number
 *          of 7's and 10's.
 * https://oeis.org/A039551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose base‑12 representation has the
 *            same positive number of digits 7 and 10
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   have_equal_sevens_tens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/have_equal_sevens_tens.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* outside the defined range */

    static seq_t *cache   = NULL;    /* array of found terms      */
    static size_t cache_len = 0;     /* number of terms stored   */
    static size_t cache_cap = 0;     /* capacity of the array    */
    static seq_t next_candidate = 94;/* first number to test     */

    /* Grow cache array as needed */
    if (cache_len == 0) {            /* initialise first run      */
        next_candidate = 94;
    }

    /* Generate terms until we have at least n of them */
    while ((idx_t)cache_len < n) {
        /* Find next valid number */
        while (true) {
            if (have_equal_sevens_tens(next_candidate)) {
                /* Ensure capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
                ++next_candidate;          /* start next search   */
                break;
            }
            ++next_candidate;
        }
    }
    return cache[n-1];
}
