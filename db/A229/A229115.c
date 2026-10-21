/*
 * A229115  Numbers n such that sigma(n) mod n - antisigma(n) mod n = 14,
 *          where sigma(n) = A000203(n) = sum of divisor of n, antisigma(n) =
 *          A024816(n) = sum of non-divisors of n.
 * https://oeis.org/A229115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1151, u32 n <= 2761, u64 n <= 2761,
 *            u128 n <= 2761, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A229115 (even n with 
    sigma(n) mod n - (n*(n+1)/2 - sigma(n)) mod n = 14)

    The implementation searches even integers n, computes sigma(n) with
    the helper `sigma_of`, evaluates the condition, and keeps a growing
    cache of found terms.  The function returns the nth such number
    independently of the order in which it is called.  */

#include <stdlib.h>                      /* malloc, realloc */
#include "../utils/sigma_of.h"           /* seq_t sigma_of(idx_t n); */

seq_t a(idx_t n)
{
    /* static cache of discovered terms */
    static seq_t *cache      = NULL;     /* array of terms */
    static size_t cache_len = 0;         /* number of terms stored */
    static size_t cache_cap = 0;         /* capacity of array      */
    static idx_t   next_cand = 2;        /* next even candidate to test */

    if (n == 0) return 0;                /* index starts at 1 */

    /* Return from cache if possible */
    if (cache_len >= n) return cache[n-1];

    /* Scan even candidates until enough terms are found */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_cand;           /* candidate value   */
        seq_t sigma = sigma_of(next_cand);        /* sum of divisors    */
        seq_t s = sigma % cand;                  /* sigma(n) mod n     */
        seq_t n_half = cand / 2;                 /* n/2                */
        seq_t anti = (n_half + cand - s) % cand;/* antisigma(n) mod n */

        if (s >= anti && (s - anti) == (seq_t)14) {
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) return 0;               /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;          /* store found term   */
        }

        next_cand += 2;                         /* next even integer */
    }

    return cache[n-1];
}
