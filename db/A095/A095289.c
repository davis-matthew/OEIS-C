/*
 * A095289  a(n) = the smallest number (in base 10) such that the product of
 *          its digits is >= n.
 * https://oeis.org/A095289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest x such that the product of x's decimal digits is
 *            ≥ n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* cache[1] will store the first term a(1).  cache[0] is unused. */
    static seq_t *cache = NULL;          /* array holding computed a(k) */
    static idx_t cap   = 0;              /* current allocated size of cache */
    static idx_t next  = 1;              /* next n value that is not stored yet */
    static seq_t  x    = 1;              /* current candidate number being examined */

    if (n == 0)                /* offset is 1; undefined for n==0, return 0 safely */
        return (seq_t)0;

    /* grow cache if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (newptr == NULL) {          /* allocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache = newptr;
        /* initialise newly allocated entries to 0; not strictly needed but tidy */
        for (idx_t i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Fill cache up to n */
    while (next <= n) {
        seq_t prod = digit_product(x);
        /* advance x until its digit product reaches the current target n */
        while (prod < next) {
            ++x;
            prod = digit_product(x);
        }
        cache[next] = x;
        ++next;
    }

    return cache[n];
}
