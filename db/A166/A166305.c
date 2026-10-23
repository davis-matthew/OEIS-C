/*
 * A166305  Even semiprimes k such that the largest prime factor + 8 is a
 *          prime. Also semiprimes k such that k+16 is semiprime.
 * https://oeis.org/A166305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * p_n where p_n is an odd prime with p_n + 8 also prime
 *            (i.e., p_n and p_n+8 form a sexy prime pair). The sequence
 *            consists of even semiprimes 2*p where both p and p+8 are prime
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
 
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache  = NULL;   /* array of a(i) values */
    static size_t cap    = 0;      /* allocated capacity */
    static size_t len    = 0;      /* number of terms stored */
    static idx_t  cand   = 3;      /* next odd integer to test (prime candidate) */
 
    /* Grow the cache until it contains at least n terms */
    while (len < n) {
        /* Test if cand is prime and cand+8 is prime */
        if (is_prime(cand) && is_prime(cand + 8)) {
            /* Ensure there is enough space */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            /* Store 2*cand as the next term */
            cache[len++] = ((seq_t) cand) * 2;
        }
        cand += 2;   /* skip even numbers */
    }
 
    return cache[n - 1];
}
