/*
 * A176301  Numbers k such that the k-th semiprime + 2 is prime.
 * https://oeis.org/A176301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that the k-th semiprime + 2 is prime
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   int_sqrt, is_prime_big, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning integers and checking semiprime status and */
/* primality of the semiprime+2. Primitive sieve for primes is used for */
/* efficient factorisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility helpers */
#include "../utils/is_prime_big.h"
#include "../utils/sieve_primes.h"
#include "../utils/int_sqrt.h"

/* -------------------------------------------------------------------- */
/* Prime table (dynamic)                                               */
/* -------------------------------------------------------------------- */
static idx_t *prime_tbl   = NULL;      /* array of primes */
static size_t prime_cnt   = 0;          /* number of primes stored */
static idx_t  prime_limit = 0;         /* current sieve limit */

/* Ensure prime_tbl contains all primes up to 'limit'. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    free(prime_tbl);
    prime_tbl = NULL;
    size_t cnt;
    sieve_primes(limit, &prime_tbl, &cnt);
    prime_limit = limit;
    prime_cnt   = cnt;
}

/* -------------------------------------------------------------------- */
/* Test whether a number is a semiprime (product of exactly two prime */
/* factors counted with multiplicity).                                  */
/* -------------------------------------------------------------------- */
static bool is_semiprime(idx_t n)
{
    if (n < 4)
        return false;

    /* Factorise n using primes up to sqrt(n). */
    idx_t temp  = n;
    int    factors = 0;
    idx_t limit   = int_sqrt(temp);
    ensure_primes(limit + 1);   /* primes needed for factorisation */

    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= limit; ++i) {
        idx_t p = prime_tbl[i];
        while (temp % p == 0) {
            ++factors;
            if (factors > 2)
                return false;
            temp /= p;
            if (temp == 1)
                break;
        }
        if (temp == 1)
            break;
    }
    if (temp > 1) {          /* remaining prime factor */
        ++factors;
        if (factors > 2)
            return false;
    }

    return factors == 2;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined, but harmless */

    idx_t found = 0;        /* how many qualifying semiprimes seen */
    idx_t semi_idx = 0;     /* index of current semiprime in the list */

    for (idx_t num = 4; ; ++num) {      /* start from the first semiprime */
        if (!is_semiprime(num))
            continue;

        ++semi_idx;                     /* we found the (semi_idx)-th semiprime */

        /* Check whether num + 2 is prime. */
        if (is_prime_big((seq_t)(num + 2))) {
            ++found;
            if (found == n)
                return (seq_t)semi_idx;
        }
    }
}
