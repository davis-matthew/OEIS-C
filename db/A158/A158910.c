/*
 * A158910  First Differences of A061240.
 * https://oeis.org/A158910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = P_{n+1} - P_n (first differences of primes ≡5 (mod 18))
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

/* Returns the n‑th term of OEIS sequence A158910.  The offset is 1. */
seq_t a(idx_t n)
{
    /* No value is defined for n == 0 – return a safe zero. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of the primes of the form 18k+5. */
    static idx_t      prime_cnt   = 0;        /* number of primes known      */
    static seq_t     *primes      = NULL;     /* array of primes              */
    static size_t     primes_cap  = 0;        /* allocated capacity of primes */
    static seq_t      next_candidate = (seq_t)5;   /* next number ≡ 5 (mod 18) */

    /* Allocate initial storage if needed. */
    if (!primes) {
        primes_cap = 20000;                  /* enough for n up to ~100 000 */
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least (n+1) of them. */
    while (prime_cnt <= n) {
        if (is_prime((idx_t)next_candidate)) {
            /* Grow the array if it is full. */
            if (prime_cnt >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[prime_cnt++] = (seq_t)next_candidate;
        }
        next_candidate += (seq_t)18;   /* next number ≡ 5 (mod 18) */
    }

    /* Return the difference between consecutive qualifying primes. */
    return primes[(size_t)n] - primes[(size_t)n - 1];
}
