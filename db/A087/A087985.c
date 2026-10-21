/*
 * A087985  a(n) = prime(x) is the smallest prime such that 1+n*prime(x) is
 *          divisible by prime(x+1), or 0 if no such prime exists.
 * https://oeis.org/A087985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that (1 + n*p) mod q == 0, where q
 *            is the next consecutive prime after p; 0 if no such prime exists
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/* Global cache of primes                                         */
/*------------------------------------------------------------------*/
static idx_t *prime_list = NULL;   /* ascending list of primes */
static size_t prime_cnt = 0;       /* number of primes stored       */
static idx_t sieve_limit = 0;      /* largest number already sieved */

/*------------------------------------------------------------------*/
/* Sieve of Eratosthenes to generate all primes up to 'limit'     */
/*------------------------------------------------------------------*/
static void generate_primes_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return; /* already done */

    /* Allocate flag array (false = prime candidate, true = composite) */
    bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_composite)
        exit(EXIT_FAILURE);   /* memory allocation failed */

    /* Mark composites */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count new primes to resize list */
    size_t new_primes = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++new_primes;

    size_t total_needed = prime_cnt + new_primes;
    if (total_needed > prime_cnt) {
        idx_t *new_list = (idx_t *)realloc(prime_list,
                                            total_needed * sizeof(idx_t));
        if (!new_list) {
            free(is_composite);
            exit(EXIT_FAILURE);
        }
        prime_list = new_list;
    }

    /* Append new primes to list */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i])
            prime_list[prime_cnt++] = i;
    }

    sieve_limit = limit;
    free(is_composite);
}

/*------------------------------------------------------------------*/
/* Ensure we have primes up to 'limit'                              */
/*------------------------------------------------------------------*/
static inline void ensure_primes(idx_t limit)
{
    if (sieve_limit < limit)
        generate_primes_up_to(limit);
}

/*------------------------------------------------------------------*/
/* Main OEIS sequence function                                      */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* invalid index, return 0 */

    /* Sieve a generous limit initially. For n up to ~10 000 the first
       few million primes are sufficient to find a solution for all
       known test cases. */
    const idx_t INITIAL_LIMIT = 10000000ULL;

    if (!sieve_limit)
        generate_primes_up_to(INITIAL_LIMIT);

    /* Search through the primes for the first satisfying the condition */
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];
        seq_t q = (seq_t)prime_list[i + 1];

        seq_t val = (seq_t)n * p + (seq_t)1;
        if (val % q == 0)
            return p;
    }

    /* No prime found in the searched range – return 0 as per definition */
    return (seq_t)0;
}
