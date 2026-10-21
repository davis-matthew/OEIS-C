/*
 * A201001  Primes in the Lucas U(3,-1) sequence.
 * https://oeis.org/A201001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes among the Lucas U(3,-1) sequence at prime indices.
   The code generates U_n = 3*U_{n-1}+U_{n-2} (U_0=0, U_1=1)
   and collects those values that are prime. The nth element of
   the returned sequence is the nth prime U_p where p itself is a
   prime index.  The implementation caches results so that
   successive calls with arbitrary order of n are efficient. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state: last index of U computed, two previous terms,
       cache of primes found, and cache capacity */
    static idx_t last_u_index = 1;        /* last n for which U_n computed */
    static seq_t u_prevprev = 0;          /* U_{last_u_index-1} */
    static seq_t u_prev     = 1;          /* U_{last_u_index}   */
    static seq_t *prime_cache = NULL;     /* array of prime U values */
    static size_t prime_cap   = 0;
    static size_t prime_len   = 0;

    /* initialise cache on first call */
    if (!prime_cache) {
        prime_cap   = 16;
        prime_cache = (seq_t*)malloc(prime_cap * sizeof(seq_t));
        prime_len   = 0;
    }

    /* extend cached primes until we have at least n values */
    while (prime_len < n) {
        /* compute next Lucas U value */
        seq_t u_cur = 3 * u_prev + u_prevprev;

        /* shift for next iteration */
        u_prevprev = u_prev;
        u_prev     = u_cur;
        last_u_index++;

        /* test primality of the newly computed value */
        if (is_prime_big(u_cur)) {
            /* grow cache if needed */
            if (prime_len == prime_cap) {
                prime_cap *= 2;
                prime_cache = (seq_t*)realloc(prime_cache,
                                              prime_cap * sizeof(seq_t));
            }
            prime_cache[prime_len++] = u_cur;
        }
    }

    /* return the requested element (1‑based index) */
    return prime_cache[n - 1];
}
