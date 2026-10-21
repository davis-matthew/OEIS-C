/*
 * A235697  Harshad numbers which when divided by their digital sum, give a
 *          quotient which is a Multiple Harshad Numbers-2.
 * https://oeis.org/A235697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 286, u32 n <= 1947, u64 n <= 1947,
 *            u128 n <= 1947, big any n
 * Helpers:   digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Harshad numbers that remain Harshad after two successive
   divisions by their digit sum (Multiple Harshad Numbers‑3).
   The function generates terms by a simple search of integers and
   caches them, so it works for any order of calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static state for incremental generation and caching */
    static seq_t *cache     = NULL;   /* pointer to array of seq_t */
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of the array */
    static seq_t next = (seq_t)1;     /* next integer to test */

    while (cache_len < n) {
        seq_t m  = next;              /* candidate number */

        /* First division: m must be divisible by its digit sum */
        seq_t d1 = digit_sum(m);
        if (d1 != (seq_t)0 && (m % d1) == (seq_t)0) {
            seq_t q1 = m / d1;

            /* Second division: q1 must be divisible by its digit sum */
            seq_t d2 = digit_sum(q1);
            if (d2 != (seq_t)0 && (q1 % d2) == (seq_t)0) {
                seq_t q2 = q1 / d2;

                /* Final check: q2 must also be a Harshad number */
                seq_t d3 = digit_sum(q2);
                if ((q2 % d3) == (seq_t)0) {
                    /* m satisfies the MHN‑3 property */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        cache = seq_realloc(cache, cache_cap, new_cap);
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = m;
                }
            }
        }
        next += (seq_t)1;
    }

    /* return the requested term */
    return cache[n - 1];
}
