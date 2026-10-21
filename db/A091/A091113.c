/*
 * A091113  Nonprimes of the form 4*k+1.
 * https://oeis.org/A091113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer of the form 4k+1 that is NOT a
 *            prime (with a(1)=1). The numbers are listed in increasing order
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* 1‑based cache of terms       */
    static idx_t cache_len = 0;     /* number of valid entries      */
    static idx_t cache_cap = 0;     /* currently allocated size    */

    if (n < 1) return (seq_t)0;     /* undefined, but avoid UB     */

    /* initialise cache for first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                    /* allocation fail          */
        cache[1] = (seq_t)1;                            /* 1 is not prime            */
        cache_len = 1;
    }

    /* extend cache until it contains the requested term */
    while (cache_len < n) {
        /* start searching from the next k after the last stored value   */
        idx_t last = (idx_t)cache[cache_len];
        idx_t k = ((last - 1) / 4) + 1;                  /* next candidate's k     */
        seq_t candidate;

        /* find the next composite 4k+1 */
        while (1) {
            candidate = (seq_t)4 * (seq_t)k + (seq_t)1;
            if (!isPrimeGeneric(candidate))
                break;
            k++;
        }

        /* grow cache if necessary */
        if (cache_len + 1 >= cache_cap) {
            cache_cap <<= 1;                     /* double the capacity    */
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation fail        */
            cache = tmp;
        }

        cache[++cache_len] = candidate;          /* store the new term     */
    }

    return cache[n];
}
