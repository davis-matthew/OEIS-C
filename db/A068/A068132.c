/*
 * A068132  Triangular numbers with sum of digits = 28.
 * https://oeis.org/A068132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digitSum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth triangular number with digit sum 28.
   Enumerate triangular numbers T(k) = k*(k+1)/2 and keep those
   whose decimal digit sum equals 28.  The sequence is stored
   in a static cache so that multiple calls are efficient. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digitSum.h"   /* digitSum(seq_t) is provided */

seq_t a(idx_t n)
{
    /* static state : cache of qualifying triangular numbers */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of elements stored */
    static size_t cap  = 0;      /* allocated capacity */
    static idx_t k_next = 1;     /* next k to test (k >= 1) */

    /* Grow cache if needed */
    if (size < n) {
        while (size < n) {
            /* compute T(k) */
            seq_t tri = (seq_t)k_next * ((seq_t)k_next + 1) / 2;
            if (digitSum(tri) == (seq_t)28) {
                /* append to cache */
                if (size == cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();            /* allocation failure */
                    cache = tmp;
                    cap = new_cap;
                }
                cache[size++] = tri;
            }
            ++k_next;
        }
    }

    return cache[n - 1];   /* a(1) is cache[0] */
}
