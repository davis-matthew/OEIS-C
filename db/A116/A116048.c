/*
 * A116048  sigma(n) times the n-th prime gives a triangular number.
 * https://oeis.org/A116048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt, get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116048:  k for which prime(k) * sigma(k) is a triangular number.
   The code searches indices k in increasing order, storing those that satisfy
   the condition to answer any query a(n). */
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static idx_t   curr   = 0;   /* last candidate k examined   */
    static seq_t  *found  = NULL;/* array of indices that satisfy */
    static size_t  sz     = 0;    /* number of stored terms       */
    static size_t  cap    = 0;    /* current capacity             */

    if (n == 0)                     /* undefined, just return 0    */
        return (seq_t)0;

    /* search until we have at least n terms ---------------------------- */
    while (sz < (size_t)n)
    {
        ++curr;                                /* candidate k = curr (1‑based) */

        seq_t prime  = (seq_t)get_prime(curr); /* pk  */
        seq_t sigma  = sigma_of(curr);       /* σ(k) */
        seq_t prod   = prime * sigma;        /* pk * σ(k) */

        /* 8*prod + 1 must be a perfect square of an odd integer   */
        seq_t tt   = prod * 8 + 1;
        seq_t ss   = floor_sqrt(tt);
        if (ss * ss == tt && (ss & 1))
        {
            /* grow storage if needed */
            if (sz == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp;

#ifdef __cplusplus
                tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
#else
                tmp = realloc(found, new_cap * sizeof(seq_t));
#endif
                if (tmp == NULL)
                    exit(EXIT_FAILURE);
                found = tmp;
                cap   = new_cap;
            }
            found[sz++] = curr;                /* store k */
        }
    }

    return found[n-1];
}
