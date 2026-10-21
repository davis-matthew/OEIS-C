/*
 * A240528  Indices of 8-almost prime triangular numbers.
 * https://oeis.org/A240528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) equals the n‑th integer m such that the triangular
   number T(m) = m(m+1)/2 has 8 prime factors (counted with multiplicity).
   Since gcd(m,m+1)=1 we have
       Ω(T(m)) = Ω(m) + Ω(m+1) − 1.
   Hence we search for m with Ω(m)+Ω(m+1) = 9. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* static cache of found indices */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;
    static idx_t   next_m = 1;   /* next m to test */

    if (cap == 0) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Grow cache until enough terms have been found */
    while (len < (size_t)n) {
        seq_t m  = (seq_t)next_m;
        seq_t mp = (seq_t)(next_m + 1);

        /* Compute Ω(m) and Ω(m+1) */
        seq_t omega_m  = count_prime_factors(m);
        seq_t omega_m1 = count_prime_factors(mp);

        /* Check the condition Ω(m)+Ω(m+1) == 9 */
        if (omega_m + omega_m1 == (seq_t)9) {
            /* Ensure capacity */
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[len++] = m;   /* store the index m */
        }

        ++next_m;
    }

    return cache[n - 1];
}
