/*
 * A106458  Bernoulli number denominators, with zeros at the odd places.
 * https://oeis.org/A106458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A106458: Bernoulli number denominators with zeros at odd indices.
 * Formula: a(n) = Π { p | p prime, (p-1) divides n }  (with a(0)=1),
 *            and a(n) = 0 for all odd n > 1.
 * The product uses only the primes p such that p-1 | n.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Prime generation ----------------------------------------------------- */
static uint64_t *primes_buf  = NULL;   /* array of primes */
static size_t     primes_cnt  = 0;     /* number of primes stored     */
static size_t     primes_cap  = 0;     /* capacity of primes_buf      */
static idx_t      sieve_limit = 0;     /* largest number up to which primes have been generated */

/* Extend the prime list up to 'limit' (inclusive).  Uses trial division. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* start at 2 if sieve_limit < 2 */
    for (idx_t cand = (sieve_limit < 2 ? 2 : sieve_limit + 1);
         cand <= limit; ++cand)
    {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes_buf[i];
            if ((idx_t)p * (idx_t)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 32;
                uint64_t *tmp = (uint64_t *)realloc(primes_buf,
                                                    primes_cap * sizeof(uint64_t));
                if (!tmp)
                    abort(); /* out of memory */
                primes_buf = tmp;
            }
            primes_buf[primes_cnt++] = cand;
        }
    }
    sieve_limit = limit;
}

/* --- Sequence function --------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* a(0) = 1; a(1) = 2; a(odd > 1) = 0 */
    if ((n & 1) && n > 1)
        return (seq_t)0;

    /* generate primes up to n+1 for candidate p values */
    ensure_primes(n + 1);

    seq_t denom = 1;

    /* loop over primes and include those with p-1 | n */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_buf[i];
        if (p > n + 1)
            break;
        if (n % (p - 1) == 0)
            denom *= (seq_t)p;
    }

    return denom;
}
