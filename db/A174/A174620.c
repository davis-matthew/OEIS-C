/*
 * A174620  a(n) = 2^A141468(n) mod prime(n), where A141468(n) is the n-th
 *          nonnegative nonprime.
 * https://oeis.org/A174620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^A141468(n) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 6600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k) */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime_small.h" /* bool is_prime_small(seq_t n) */

static seq_t *nonprimes = NULL;   /* cache of non‑prime integers (0,1,4,6,…) */
static size_t np_cnt   = 0;       /* number of cached non‑primes          */
static size_t np_cap   = 0;       /* current capacity of the cache        */
static idx_t  last_seen = 0;      /* next integer to test for non‑prime   */

static void grow_nonprimes(void)
{
    size_t new_cap = np_cap == 0 ? 128 : np_cap << 1;
    seq_t *tmp = (seq_t *)realloc(nonprimes, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* cannot allocate more memory  */
    nonprimes = tmp;
    np_cap    = new_cap;
}

/* Fill the cache with at least n non‑prime values. */
static void ensure_nonprimes(idx_t n)
{
    while (np_cnt < (size_t)n) {
        seq_t val = (seq_t)last_seen;
        if (!is_prime_small(val)) {
            if (np_cnt == np_cap) grow_nonprimes();
            nonprimes[np_cnt++] = val;
        }
        ++last_seen;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* safety guard – OEIS offset is 1 */

    /* (1)  Get the n‑th non‑prime (A141468). */
    ensure_nonprimes(n);
    seq_t np_val = nonprimes[(size_t)(n - 1)]; /* 0‑based array   */

    /* (2)  Get the n‑th prime. */
    uint32_t p32 = get_prime(n);               /* prime fits in 32 bits  */
    seq_t prime  = (seq_t)p32;                 /* cast to seq_t           */

    /* (3)  Compute 2^np_val  (mod prime). */
    return mod_pow((seq_t)2, (uint64_t)np_val, prime);
}
