/*
 * A141079  a(n)=abs(p(n)*p(n+3)-p(n+1)*p(n+2)), where p(n)=n-th prime.
 * https://oeis.org/A141079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs( p(n)*p(n+3) - p(n+1)*p(n+2) ) (n ≥ 1), where p(k) is
 *            the k‑th prime
 * Range:     u8 n <= 12, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* a static cache of the first primes generated so far */
    static uint32_t *primes = NULL;
    static size_t    primes_cnt = 0;

    /* We need at least n+3 primes (1‑based). */
    if (primes_cnt < (size_t)n + 3)
    {
        /* release any previously allocated cache */
        if (primes) free(primes);

        /* generate the necessary first primes */
        size_t needed = (size_t)n + 3;          /* number of primes requested */
        uint32_t *tmp = NULL;
        size_t    cnt;
        sieve_first_n_primes(needed, &tmp, &cnt);   /* Helper from utils */

        /* store the new cache for future calls */
        primes     = tmp;
        primes_cnt = cnt;
    }

    /* fetch the required primes (array is 0‑based) */
    seq_t pn  = (seq_t)primes[n - 1];
    seq_t pn1 = (seq_t)primes[n];
    seq_t pn2 = (seq_t)primes[n + 1];
    seq_t pn3 = (seq_t)primes[n + 2];

    /* compute the two products and their absolute difference */
    seq_t prod1 = pn  * pn3;   /* p(n)   * p(n+3) */
    seq_t prod2 = pn1 * pn2;   /* p(n+1) * p(n+2) */

    return (prod1 >= prod2) ? (prod1 - prod2) : (prod2 - prod1);
}
