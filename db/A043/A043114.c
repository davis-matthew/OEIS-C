/*
 * A043114  Numbers k such that 2 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043114: Numbers k such that 2 and 4 appear next to each other
   (in either order) in the base‑5 representation of k, but do not appear
   next to each other in k‑1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Two static arrays are used to cache already discovered terms: */
static seq_t *cache      = NULL;   /* a(1), a(2), …                  */
static size_t cached      = 0;      /* number of elements stored      */
static size_t cache_cap   = 0;      /* capacity of cache array        */
static seq_t next_to_test = 1;      /* smallest k not yet examined    */

/* Return true if the base‑5 representation of x contains the
   adjacent digits 2 and 4 in either order (i.e. '24' or '42'). */
static bool has_adjacent_2_4(seq_t x)
{
    if (x == 0) return false;              /* base‑5 of 0 is “0”            */

    /* Store digits from least significant to most significant */
    uint8_t digs[200];
    size_t len = 0;
    seq_t five = (seq_t)5;

    while (x > 0) {
        digs[len++] = (uint8_t)(x % five);   /* least‑significant digit   */
        x /= five;                          /* shift right in base‑5     */
    }

    /* Check for '24' or '42' in the normal left‑to‑right order.
       In the array we must look for 2 followed by 4 (digs[j]==2 && digs[j-1]==4)
       or 4 followed by 2 (digs[j]==4 && digs[j-1]==2). */
    for (size_t j = 1; j < len; ++j) {
        if ((digs[j] == 2 && digs[j-1] == 4) ||
            (digs[j] == 4 && digs[j-1] == 2))
            return true;
    }
    return false;
}

/* Return the nth term of the sequence (a(1) = 14). */
seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n terms. */
    while (cached < n) {
        seq_t k = next_to_test;

        if (has_adjacent_2_4(k) && !has_adjacent_2_4(k - 1)) {
            /* Ensure enough space for the next cached value. */
            if (cached >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* allocation error */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cached++] = k;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}
