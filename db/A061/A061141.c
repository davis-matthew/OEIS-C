/*
 * A061141  Compute sum of divisors of the prime(n+1)-prime(n)-1 composite
 *          numbers between two consecutive primes; choose the largest.
 * https://oeis.org/A061141
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2026, u32 n <= 9681, u64 n <= 9681,
 *            u128 n <= 9681, big any n
 * Helpers:   get_prime, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Max{sigma(c) | p(n+1) > c > p(n), c composite}          */
/* (OEIS A061141).  The function uses the supplied helpers get_prime,    */
/* is_prime, and sigma_of to avoid rebuilding large prime tables.          */

#include <stdbool.h>
/* Helper functions provided by the framework */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */
#include "../utils/is_prime.h"    /* bool is_prime(idx_t n) */
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n) */

/* --------------------------------------------------------------------- */
/* Main sequence function.  It is thread‑safe only through static data,  */
/* but the problem statement prohibits external state, so we keep it      */
/* stateless: each call queries the helpers directly.                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)     /* undefined below the offset; return 0 as a neutral value */
        return 0;

    /* Retrieve the two consecutive primes.  get_prime(1) is the first prime. */
    idx_t p_n    = (idx_t)get_prime(n);     /* p(n)   */
    idx_t p_next = (idx_t)get_prime(n + 1); /* p(n+1) */

    seq_t max_sigma = 0;

    /* Scan all integers strictly between the two primes. */
    for (idx_t c = p_n + 1; c < p_next; ++c) {
        /* Skip primes – only composites count. */
        if (is_prime(c))
            continue;

        seq_t s = sigma_of(c);     /* sum of divisors of c */
        if (s > max_sigma)
            max_sigma = s;
    }

    return max_sigma;
}
