/*
 * A029767  a(n) = (n-1)!*(2^n-1) for n>=1, a(0)=0.
 * https://oeis.org/A029767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * (2^n - 1) (with a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0) return (seq_t)0;

    /* We compute (n-1)! lazily and cache it */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cache_size = 0;   /* number of stored factorials,
                                           fact_cache[i] == i!   */

    idx_t needed = n - 1;   /* factorial that we need */

    if (fact_cache_size <= needed) {
        idx_t new_cap = needed + 1;
        fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!fact_cache) exit(EXIT_FAILURE);      /* OOM */

        /* Resolve factorial values that have not yet been computed */
        for (idx_t i = fact_cache_size; i <= needed; ++i) {
            if (i == 0)
                fact_cache[0] = (seq_t)1;
            else
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_size = new_cap;
    }

    seq_t fact = fact_cache[needed];      /* (n-1)! */
    seq_t two_n = pow2(n);                /* 2^n  */
    seq_t res   = fact * (two_n - (seq_t)1);
    return res;
}
