/*
 * A062357  a(n) = n*p(n+1)-(n+1)*p(n) = n*d(n)-p(n), where p(n) is the n-th
 *          prime and d(n) is the n-th prime-difference, A001223(n).
 * https://oeis.org/A062357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*p(n+1) - (n+1)*p(n), where p(k) is the k‑th prime
 * Range:     i8 n <= 29, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*   Internal state used to cache the prime table.   */
static idx_t      sieve_limit = 0;     /* the upper bound used in the sieve       */
static idx_t     *prime_table = NULL;  /* contiguous array of primes up to sieve_limit */
static size_t      prime_count = 0;    /* number of primes stored in prime_table  */

/* ------------------------------------------------------------------- */
/*  Allocate a prime table containing at least max_n primes.           */
/* ------------------------------------------------------------------- */
static void init_prime_table(idx_t max_n)
{
    /*  If we already have enough primes, nothing to do.                 */
    if (prime_count >= max_n)
        return;

    /*  Choose an appropriate upper bound for the sieve.  The
        prime number theorem gives p_n ≈ n log n, we add a safety margin.  */
    if (sieve_limit == 0) {
        /*  For n up to about 100 000 the 100 001st prime is < 1.3 M.  */
        sieve_limit = 1300000;
    } else {
        /*  In case more primes are needed, increase the bound.           */
        sieve_limit *= 2;
    }

    /*  Allocate and run a simple Eratosthenes sieve.                     */
    size_t sz = (size_t)(sieve_limit + 1);
    char *is_comp = (char *)malloc(sz);
    if (!is_comp)
        exit(EXIT_FAILURE);

    is_comp[0] = is_comp[1] = 1;
    for (idx_t i = 2; i * i <= sieve_limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= sieve_limit; j += i)
                is_comp[j] = 1;
        }
    }

    /*  Count primes to allocate exact space.                            */
    size_t cnt = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i)
        if (!is_comp[i])
            ++cnt;

    /*  Free old table if it exists.  */
    if (prime_table) {
        free(prime_table);
    }

    prime_table  = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!prime_table)
        exit(EXIT_FAILURE);

    /*  Store the primes.                                                */
    size_t idx = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (!is_comp[i]) {
            prime_table[idx++] = i;
        }
    }
    prime_count = cnt;
    free(is_comp);
}

/* ------------------------------------------------------------------- */
/*  Grow the table with an incremental prime generator when the       */
/*  sieve bound is insufficient.  This is rarely needed for the        */
/*  typical test ranges.                                               */
/* ------------------------------------------------------------------- */
static void grow_prime_table(idx_t required)
{
    if (prime_count >= required)
        return;

    idx_t candidate = sieve_limit + 1;
    while (prime_count < required) {
        bool is_prime = true;
        /*  Check divisibility only up to sqrt(candidate).                 */
        for (size_t j = 0; j < prime_count && prime_table[j] * prime_table[j] <= candidate; ++j) {
            if (candidate % prime_table[j] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /*  Append new prime.                                         */
            idx_t *new_tbl = (idx_t *)realloc(prime_table, (prime_count + 1) * sizeof(idx_t));
            if (!new_tbl) exit(EXIT_FAILURE);
            prime_table = new_tbl;
            prime_table[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------- */
/*  Public function that implements the sequence.                      */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)         /* the sequence is defined for n≥1; 0 is undefined */
        return 0;

    /*  Ensure that the prime table contains at least n+1 primes.  */
    init_prime_table(n + 1);
    if (prime_count < n + 1)
        grow_prime_table(n + 1);

    /*  Retrieve the required primes.  Remember that prime_table is
        zero‑based: prime_table[0] = p(1).                            */
    seq_t pn  = (seq_t)prime_table[n - 1];   /* p(n)   */
    seq_t pn1 = (seq_t)prime_table[n];       /* p(n+1) */

    /*  Compute the formula using seq_t arithmetic.                    */
    seq_t result = (seq_t)n * pn1 - (seq_t)(n + 1) * pn;
    return result;
}
