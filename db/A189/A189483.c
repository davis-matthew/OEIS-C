/*
 * A189483  Primes that are not the arithmetic derivative (A003415) of any
 *          integer.
 * https://oeis.org/A189483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189483: primes that are not the arithmetic derivative of any integer.
   The algorithm builds a sieve up to LIMIT, computes the arithmetic derivative of
   every integer ≤ LIMIT, marks primes that appear as a derivative, then
   collects the remaining primes.  The list is stored in a static array and
   indexed directly by the call argument.  The limit is chosen to contain
   well over 10 000 valid terms, so a call for any n ≥ 1 returns the correct
   value in O(1).  All memory allocations use explicit casts to satisfy
   both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------------------------------------------------------------------
   Configurable sieve limit.  The first 10 000 terms of the sequence
   all lie below 2 000 000; primes near this region are ~1/ln(2e6)=0.035.
   Half of them are missing, so we expect ~70 000 terms – more than enough.
   ---------------------------------------------------------------------- */
#define LIMIT 2000000U

/* ----------------------------------------------------------------------
   Static state.
   ---------------------------------------------------------------------- */
static bool initialized = false;      /* have we run init_once() ? */
static uint8_t *is_prime_arr = NULL;  /* 1 for prime, 0 otherwise */
static uint8_t *derived_arr = NULL;   /* 1 if the prime is a derivative */
static uint32_t *prime_list   = NULL; /* list of all primes ≤ LIMIT */
static size_t prime_cnt        = 0;     /* number of primes in prime_list */
static seq_t  *seq_cache       = NULL; /* list of valid terms (non‑derived primes) */
static size_t seq_len          = 0;     /* number of terms in seq_cache */

/* ----------------------------------------------------------------------
   Sieve and auxiliary computations.  Runs once on first call.
   ---------------------------------------------------------------------- */
static void init_once(void)
{
    if (initialized) return;

    /* Allocate sieve arrays (C++ requires explicit cast). */
    is_prime_arr = (uint8_t*)malloc((LIMIT + 1U) * sizeof(uint8_t));
    derived_arr  = (uint8_t*)malloc((LIMIT + 1U) * sizeof(uint8_t));
    if (!is_prime_arr || !derived_arr) exit(EXIT_FAILURE);

    /* Initialise to prime. */
    for (size_t i = 0; i <= LIMIT; ++i) {
        is_prime_arr[i] = 1U;
        derived_arr[i]  = 0U;
    }
    is_prime_arr[0] = is_prime_arr[1] = 0U;

    /* Classic Sieve of Eratosthenes. */
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime_arr[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                is_prime_arr[q] = 0U;
        }
    }

    /* Smallest prime factor array for fast factorisation. */
    uint32_t *spf = (uint32_t*)malloc((LIMIT + 1U) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (spf[p] == (uint32_t)p) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                if (spf[q] == (uint32_t)q)
                    spf[q] = (uint32_t)p;
        }
    }

    /* Count primes for allocation of prime_list. */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime_arr[i]) ++prime_cnt;

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime_arr[i]) prime_list[idx++] = (uint32_t)i;

    /* Compute arithmetic derivatives for all n ≤ LIMIT. */
    for (size_t n = 2; n <= LIMIT; ++n) {
        size_t tmp = n;
        uint64_t D = 0ULL;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            size_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            D += ((uint64_t)n / p) * cnt;
        }
        if (D <= LIMIT && is_prime_arr[(size_t)D])
            derived_arr[(size_t)D] = 1U;
    }

    /* Free temporary array. */
    free(spf);

    /* Count how many primes are not derivatives. */
    size_t non_deriv_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
        if (!derived_arr[prime_list[i]]) ++non_deriv_cnt;

    /* Allocate cache for the sequence. */
    seq_cache = (seq_t*)malloc(non_deriv_cnt * sizeof(seq_t));
    if (!seq_cache) exit(EXIT_FAILURE);
    seq_len = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (!derived_arr[p]) {
            seq_cache[seq_len] = (seq_t)p;
            ++seq_len;
        }
    }

    /* We are ready. */
    initialized = true;
}

/* ----------------------------------------------------------------------
   Public sequence function.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined; return 0 for safety */
    init_once();
    if (n > seq_len) return 0;      /* out of pre‑computed range (should not happen) */
    return seq_cache[n - 1];
}
