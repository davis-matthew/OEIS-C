/*
 * A159815  Numbers n with property that n^2 starts and ends with 49.
 * https://oeis.org/A159815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159815: numbers n such that n^2 starts and ends with 49. */
/* The function generates terms in increasing order. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static size_t      cache_len = 0;      /* number of terms stored      */
    static size_t      cache_cap = 0;      /* current capacity of array   */
    static seq_t       *cache    = NULL;   /* array of terms              */
    static seq_t       next     = 7;       /* next integer to test        */

    if (n == 0) return 0;      /* undefined for n = 0, return 0 */

    /* Generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* search for the next integer whose square satisfies the property */
        for (;;) {
            seq_t sq = next * next;
            if ((sq % (seq_t)100) == (seq_t)49) {
                size_t d = digit_len(sq);
                if (d >= 2) {
                    seq_t div  = pow10((idx_t)(d - 2));
                    seq_t lead = sq / div;
                    if (lead == (seq_t)49) {
                        /* store the found term */
                        append_candidate(&cache, &cache_len, &cache_cap, next);
                        ++next;        /* move to the next candidate */
                        break;         /* proceed to generate the next cache entry */
                    }
                }
            }
            ++next;                 /* try the following integer */
        }
    }

    return cache[(size_t)n - 1];
}
