/*
 * A013920  Composite numbers k such that the sum of all composites <= k is
 *          composite.
 * https://oeis.org/A013920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A013920: Composite numbers k such that the sum of all
 * composites ≤ k is composite.
 *
 * The routine generates the terms lazily.  It iterates over natural
 * numbers, maintains a running sum of composites, and records a
 * number only when that sum itself is composite.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms      = NULL;   /* array of found terms   */
    static idx_t  terms_len  = 0;       /* number of terms stored */
    static idx_t  terms_cap  = 0;       /* allocated capacity     */
    static idx_t  last       = 3;       /* last tested number     */
    static seq_t  cum_sum    = 0;       /* cumulative sum of composites */

    /* nothing to return for n < 1 (undefined by OEIS) */
    if (n < 1) return 0;

    /* enlarge buffer as necessary */
    if (!terms || terms_len == terms_cap) {
        idx_t new_cap = terms_cap? terms_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
        if (!tmp) abort();                  /* allocation failed */
        terms      = tmp;
        terms_cap  = new_cap;
    }

    /* generate terms until we have at least n */
    while (terms_len < n) {
        ++last;                             /* next candidate */

        /* test if last is composite (i.e., >1 and not prime) */
        if (last > 1 && !is_prime_small((seq_t)last)) {
            cum_sum += (seq_t)last;           /* add to running sum */

            /* check if the running sum is also composite */
            if (cum_sum > (seq_t)1 && !is_prime_small(cum_sum)) {
                terms[terms_len++] = last;  /* store term */
            }
        }
    }

    return terms[n - 1];
}
