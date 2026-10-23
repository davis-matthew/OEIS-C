/*
 * A135027  Numbers k such that the sum of the digits of k^2 is 10. Multiples
 *          of 10 are omitted.
 * https://oeis.org/A135027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 20, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A135027: numbers k such that the sum of the
 * digits of k² equals 10, with k not a multiple of 10.
 * The terms are generated on demand by an incremental search.
 * The function keeps a static cache so that repeated calls are fast.
 */

#include <stddef.h>     /* size_t, NULL */
#include <stdlib.h>     /* malloc, realloc */
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t) */

seq_t a(idx_t n)
{
    /* Static state for incremental generation */
    static idx_t       next_k = 1;      /* next k to test                */
    static size_t      len    = 0;      /* number of terms found so far   */
    static size_t      cap    = 16;     /* current capacity of cache      */
    static seq_t      *cache  = NULL;   /* array of found terms           */

    /* Allocate cache on first use */
    if (!cache) {
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        /* Skip multiples of 10 as required */
        if ((next_k % 10) != 0) {
            seq_t kseq = (seq_t)next_k;
            seq_t sq   = kseq * kseq;   /* k² */

            if (digit_sum(sq) == (seq_t)10) {
                /* Grow cache if full */
                if (len == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = kseq;   /* store term */
            }
        }
        ++next_k;  /* advance to next possible k */
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
