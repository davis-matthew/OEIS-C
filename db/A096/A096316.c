/*
 * A096316  Given the number wheel 0,1,2,3,4,5,6,7,8,9 then starting with 2,
 *          the next number is a prime p number of positions from the previous
 *          number found, for p=2,3,...
 * https://oeis.org/A096316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2 + Σ_{k=0}^{n} p_k) mod 10, where p_k is the k‑th prime
 *            (p_0 = 2)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/* use the prime sieve routine supplied by the framework */
#include "../utils/build_primes.h"

/* Static state --------------------------------------------------------- */

static idx_t   *prime_list   = NULL;   /* array of primes up to current_limit */
static size_t  prime_cnt     = 0;      /* number of primes stored          */
static idx_t   current_limit = 0;      /* current sieve limit              */
static uint8_t *cum_mod10    = NULL;   /* cumulative sum modulo 10         */

/* Ensure that we have at least (n+1) primes in prime_list.                */
static void
ensure_primes_for(idx_t n)
{
    if (prime_cnt > n)
        return;   /* already enough */

    /* Initial limit – small, will grow as needed */
    if (current_limit == 0)
        current_limit = 200;          /* start with primes <= 200 */

    /* Grow the sieve until we have enough primes */
    while (prime_cnt <= n) {
        idx_t   new_limit = current_limit * 2;
        idx_t  *new_list  = NULL;
        size_t  new_cnt   = 0;

        /* build primes up to new_limit */
        build_primes(new_limit, &new_list, &new_cnt);

        /* discard previous data */
        free(prime_list);
        free(cum_mod10);

        prime_list   = new_list;
        prime_cnt    = new_cnt;
        current_limit = new_limit;

        /* recompute cumulative sum modulo 10 */
        cum_mod10 = (uint8_t *)malloc(prime_cnt * sizeof(uint8_t));
        if (!cum_mod10) {
            /* out of memory – terminate – we can't recover safely */
            exit(EXIT_FAILURE);
        }
        uint8_t acc = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            acc = (acc + (uint8_t)(prime_list[i] % 10)) % 10;
            cum_mod10[i] = acc;
        }
    }
}

/* --------------------------------------------------------------------- */
/* The requested function                                                  */
seq_t
a(idx_t n)
{
    ensure_primes_for(n);

    /* cum_mod10[n] = Σ_{k=0}^{n} p_k (mod 10) */
    uint8_t sum_mod = (uint8_t)((2 + cum_mod10[n]) % 10);
    return (seq_t)sum_mod;
}
