/*
 * A180083  Smallest k such that digit sum of k > previous term.
 * https://oeis.org/A180083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   min_with_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A180083:  Smallest k such that digit sum of k > previous term.
 *  Recurrence: a(1) = 0
 *  For n > 1: a(n) = the smallest integer > a(n-1) whose digit sum exceeds a(n-1).
 *  This is equivalent to: a(n) = minimal integer with digit sum = a(n-1)+1.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/min_with_digit_sum.h"

/*  Cached computation of a(n).  The result for each n is stored in an
 *  array; if subsequent calls request larger n the array is expanded
 *  incrementally.  This allows calls in any order without recomputation
 *  of earlier terms.                                               */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of precomputed terms   */
    static idx_t cache_cap = 0;   /* allocated size (in seq_t units) */

    if (n == (idx_t)1) {
        return (seq_t)0;
    }

    /* Ensure cache big enough */
    if (cache_len == 0) {
        /* initialise with a(1) = 0 */
        cache_cap = 1;
        cache    = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        seq_t S = prev + 1;            /* digit sum needed for next term */
        seq_t next = min_with_digit_sum(S);

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
