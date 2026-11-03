/*
 * A180083  Smallest k such that digit sum of k > previous term.
 * https://oeis.org/A180083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/*  Helper that constructs the minimal number having a given digit sum.
 *  For a digit sum S, the minimal number is:
 *      - if S % 9 == 0:   99...9   (S/9 copies of '9')
 *      - otherwise:      (S%9) followed by (S/9) copies of '9'
 */
static seq_t min_with_digit_sum(seq_t S)
{
    seq_t q = S / 9;          /* number of '9' digits        */
    unsigned r = (unsigned)(S % 9);   /* first digit if non‑zero     */
    seq_t res = 0;

    if (r > 0) {
        res = r;
    }

    /* append q copies of 9 */
    /* Note: for the sequence values that fit in seq_t, q is small enough
     * to finish in a reasonable time.  For larger q the value would overflow
     * seq_t anyway, which is outside the test range.            */
    for (idx_t j = 0; j < (idx_t)q; ++j) {
        res = res * 10 + 9;
    }
    return res;
}

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
        cache    = mem_new(seq_t, cache_cap);
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
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
