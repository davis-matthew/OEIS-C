/*
 * A049011  Composite numbers k such that number of composite d with 3 < d <
 *          k, gcd(k, d) = 1, is pi(k).
 * https://oeis.org/A049011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   alloc_mem, realloc_mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth composite number k that satisfies
   φ(k) + ω(k) == 2·π(k) + 1, where
   φ  is Euler's totient,
   ω  is the number of distinct prime factors,
   π  is the prime counting function. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_mem.h"
#include "../utils/realloc_mem.h"

/* --------------------------------------------------------------------- */
/* /!\ The build supplies ⎢seq_t⎥ and ⎢idx_t⎥ types.                       */
/* --------------------------------------------------------------------- */

/* Sieve limits -------------------------------------------------------- */
#define SIEVE_LIMIT 10000000ULL   /* increase if more terms are required */

/* Static data for the sieve ------------------------------------------ */
static bool       *is_prime   = NULL;   /* is_prime[i] == true iff i is prime      */
static uint32_t   *phi_arr    = NULL;   /* φ(i)                                     */
static uint8_t    *omega_arr  = NULL;   /* ω(i)                                     */
static uint32_t   *pi_arr     = NULL;   /* π(i) on prefix                         */
static uint32_t   *primes     = NULL;   /* list of primes up to SIEVE_LIMIT       */
static size_t      primes_cnt = 0;       /* number of primes found                  */
static bool        sieve_done = false;  /* has the sieve been built?               */

/* Cache for sequence values ----------------------------------------- */
static seq_t   *seq_cache = NULL;   /* array of found terms */
static size_t   cache_cap  = 0;     /* capacity of seq_cache    */
static size_t   cache_len  = 0;     /* number of terms stored   */
static idx_t    last_k     = 26;    /* k just before first term (27) */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Builds a linear sieve up to SIEVE_LIMIT, filling arrays above. */
static void build_sieve(void)
{
    if (sieve_done)
        return;
    sieve_done = true;

    /* Allocate arrays ---------------------------------------------- */
    is_prime  = (bool*)alloc_mem((SIEVE_LIMIT + 1) * sizeof(bool));
    phi_arr   = (uint32_t*)alloc_mem((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    omega_arr = (uint8_t*)   alloc_mem((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    pi_arr    = (uint32_t*)alloc_mem((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    primes    = (uint32_t*)alloc_mem((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!is_prime || !phi_arr || !omega_arr || !pi_arr || !primes) {
        /* In a real program we might handle this, but we simply abort. */
        exit(EXIT_FAILURE);
    }

    /* init -------------------------------- --------------------------- */
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) {
        is_prime[i] = true;
        phi_arr[i]  = 0;
        omega_arr[i]= 0;
    }
    is_prime[0] = is_prime[1] = false;

    /* Linear sieve for φ and ω -------------------------------------- */
    phi_arr[1]   = 1;
    omega_arr[1] = 0;
    primes_cnt   = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            phi_arr[i]   = (uint32_t)(i - 1);
            omega_arr[i] = 1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t mult = (uint64_t)i * p;
            if (mult > SIEVE_LIMIT) break;
            idx_t idx = (idx_t)mult;
            is_prime[idx] = false;
            if (i % p == 0) {
                phi_arr[idx]   = phi_arr[i] * p;
                omega_arr[idx] = omega_arr[i];
                break;
            } else {
                phi_arr[idx]   = phi_arr[i] * (p - 1);
                omega_arr[idx] = omega_arr[i] + 1;
            }
        }
    }

    /* Build π prefix ------------------------------ */
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= SIEVE_LIMIT; ++i) {
        pi_arr[i] = pi_arr[i-1] + (is_prime[i] ? 1 : 0);
    }
}

/* --------------------------------------------------------------------- */
/* Helper to enlarge the sequence cache -------------------------------- */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc_mem(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of range – not expected */

    build_sieve();

    /* Grow cache until we have at least n entries --------------------- */
    while (cache_len < (size_t)n) {
        /* Search for the next composite k satisfying the condition ----- */
        idx_t k = last_k + 1;
        for (; k <= SIEVE_LIMIT; ++k) {
            if (is_prime[(size_t)k]) continue;          /* not composite */
            seq_t phi   = (seq_t)phi_arr[(size_t)k];
            seq_t omega = (seq_t)omega_arr[(size_t)k];
            seq_t pi_val= (seq_t)pi_arr[(size_t)k];
            if (phi + omega == 2 * pi_val + 1) {
                ensure_cache_cap(cache_len + 1);
                seq_cache[cache_len++] = (seq_t)k;
                last_k = k;
                break;
            }
        }
        if (k > SIEVE_LIMIT) {
            /* Unexpected: term lies beyond pre‑computed limit.  In a full
               implementation we could rebuild a larger sieve.  Here we
               simply abort. */
            exit(EXIT_FAILURE);
        }
    }

    return seq_cache[(size_t)n - 1];
}
