/*
 * A175587  a(1) = 4; a(n) = smallest semiprime such that a(n) - a(n-1) is
 *          prime.
 * https://oeis.org/A175587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175587
   a(1) = 4
   a(n) = smallest semiprime greater than a(n‑1) whose difference from
          a(n‑1) is prime.
   This implementation generates primes up to a fixed limit,
   tests each successive integer for semiprimality, and uses
   is_prime_big() to test the required difference. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

/* If the sequence values exceed  4,000,000 we re‑grow the prime list.
   The chosen limit gives plenty of semiprimes well beyond n = 10 000. */
#define PRIME_LIMIT 2000000U

/* Static state */
static bool     initialized = false;
static seq_t   *a_cache     = NULL;
static size_t   a_cap       = 0;
static idx_t    a_len       = 0;

static uint64_t *primes_u64 = NULL;
static size_t    prime_cnt  = 0;

/* ------------------------------------------------------------------ */
/* Helper: test whether x is a semiprime (product of exactly two primes)
   using the precomputed list of small primes. */
static bool
is_semiprime(seq_t x)
{
    seq_t temp = x;
    int   cnt  = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes_u64[i];
        seq_t   pp = (seq_t)p * (seq_t)p;
        if (pp > temp)
            break;

        while (temp % (seq_t)p == 0)
        {
            temp /= (seq_t)p;
            ++cnt;
            if (cnt > 2)
                return false;
        }
    }
    /* If temp > 1, it is a prime factor > sqrt(original). */
    if (temp > 1)
        ++cnt;

    return cnt == 2;
}

/* ------------------------------------------------------------------ */
/* Initialise the global tables and the first sequence value. */
static void
init_global_state(void)
{
    /* Build a list of primes up to PRIME_LIMIT. */
    build_primes_upto(PRIME_LIMIT, &primes_u64, &prime_cnt);

    /* Allocate space for the sequence cache. */
    a_cap = 64;
    a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
    if (!a_cache)
        return;  /* In a real environment we would handle the error. */

    a_cache[0] = (seq_t)4;                /* a(1) = 4 */
    a_len = 1;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Compute the sequence up to index n (inclusive). */
static void
ensure_sequence_upto(idx_t n)
{
    for (idx_t idx = a_len; idx < n; ++idx)
    {
        seq_t prev = a_cache[idx - 1];
        seq_t cand = prev + (seq_t)1;

        /* Seek the next semiprime whose difference from prev is prime. */
        while (true)
        {
            if (is_semiprime(cand))
            {
                seq_t diff = cand - prev;
                if (is_prime_big(diff))
                {
                    /* Append to cache. */
                    if (idx >= a_cap)
                        ensure_capacity(idx + 1, &a_cap, (void **)&a_cache,
                                        sizeof(seq_t));
                    a_cache[idx] = cand;
                    break;
                }
            }
            ++cand;
        }
    }
    a_len = n;
}

/* ------------------------------------------------------------------ */
/* Public API required by the harness. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_global_state();

    /* Our implementation ignores calls with n == 0. */
    if (n == 0)
        return (seq_t)0;

    if ((idx_t)a_len < n)
        ensure_sequence_upto(n);

    return a_cache[n - 1];
}
