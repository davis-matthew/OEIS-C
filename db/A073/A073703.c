/*
 * A073703  Smallest prime p such that also p+prime(n)*2 is a prime.
 * https://oeis.org/A073703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2828, u32 n <= 2876, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements the OEIS definition for A073703:
    a(n) = least prime p such that p + 2*prime(n) is prime.
    It searches over primes, expanding the prime sieve as needed.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"       /* uint32_t get_prime(idx_t k);  */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n);  */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                         uint64_t **primes_out, size_t *cnt_out); */

static uint64_t *shared_primes   = NULL; /* array of primes */
static size_t   shared_prime_cnt = 0;
static idx_t    shared_limit     = 0;    /* current sieve upper bound */

/* Rebuild prime list up to at least <limit> if required */
static void ensure_primes_upto(idx_t limit)
{
    if (shared_limit >= limit)
        return;

    if (shared_primes) free(shared_primes);
    build_primes_upto(limit, &shared_primes, &shared_prime_cnt);
    shared_limit = limit;
}

/*------------------------------------------------------------------*/
/* Main sequence function                                            */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 1. Get the n-th prime (OEIS indexing starts at 1) */
    uint32_t pn = get_prime(n);          /* prime(n)  */

    /* 2. Compute offset 2*prime(n) */
    seq_t d = (seq_t)pn * 2U;

    /* 3. Ensure we have a prime list ready (start with 1 000 000) */
    if (shared_limit == 0)
        ensure_primes_upto((idx_t)1000000ULL);

    /* 4. Search for the smallest prime p with p + d prime */
    for (;;)
    {
        for (size_t i = 0; i < shared_prime_cnt; ++i)
        {
            seq_t p   = (seq_t)shared_primes[i];
            seq_t sum = p + d;
            if (is_prime_big(sum))
                return p;
        }

        /* No match within current sieve, double the limit and rebuild. */
        idx_t new_limit = shared_limit * (idx_t)2;
        ensure_primes_upto(new_limit);
    }
}
