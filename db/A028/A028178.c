/*
 * A028178  Expansion of 1/((1-5x)(1-6x)(1-10x)(1-12x)).
 * https://oeis.org/A028178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 33*a(n-1)-392*a(n-2)+1980*a(n-3)-3600*a(n-4),
 * with initial values a(0)=1, a(1)=33, a(2)=697, a(3)=12045.
 * The implementation memoises the sequence in a static dynamic array,
 * so repeated calls are fast and order-independent.

 * Credits: OEIS A028178 – Expansion of 1/((1-5x)(1-6x)(1-10x)(1-12x)). */

#include <stdlib.h>

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;      /* dynamically allocated array */
static idx_t cache_size = 0;      /* number of elements currently stored */

/* Compute a(n) using the four‑term linear recurrence. */
seq_t a(idx_t n)
{
    /* Grow the cache if required. */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) {
            /* Allocation failed – abort (no special error handling). */
            abort();
        }
        cache = tmp;
        /* Initialise any new entries.  For the first four indices
           we use the given initial values; afterwards the recurrence
           is used, grouping positive and negative terms to avoid
           intermediate underflow. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 4) {
                switch (i) {
                    case 0: cache[0] = (seq_t)1;          break;
                    case 1: cache[1] = (seq_t)33;         break;
                    case 2: cache[2] = (seq_t)697;        break;
                    case 3: cache[3] = (seq_t)12045;      break;
                }
            } else {
                seq_t term1 = (seq_t)33       * cache[i-1];
                seq_t term2 = (seq_t)1980     * cache[i-3];
                seq_t term3 = (seq_t)392      * cache[i-2];
                seq_t term4 = (seq_t)3600     * cache[i-4];
                seq_t pos   = term1 + term2;          /* positive part   */
                seq_t neg   = term3 + term4;          /* negative part   */
                cache[i] = pos - neg;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
