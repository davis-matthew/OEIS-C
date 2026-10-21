/*
 * A079131  Primes such that iterated sum-of-digits (A038194) is odd.
 * https://oeis.org/A079131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth prime p such that the iterated sum‑of‑digits *
 *            (digital root) of p is odd. This is equivalent to p ≡ 1,3,5,7
 *            (mod 18)
 * Range:     u8 n <= 26, u16 n <= 3261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper to test if a small integer is prime by trial division.
 * Assumes p > 1. */
static bool is_prime_uint32(uint32_t p, const uint32_t *primes, size_t prime_cnt)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t q = primes[i];
        if ((uint64_t)q * q > p)
            break;
        if (p % q == 0)
            return false;
    }
    return true;
}

/* Global state: cache of primes satisfying the condition */
static seq_t *valid_primes = NULL;         /* array of seq_t (primes) */
static size_t valid_len   = 0;              /* number of stored primes   */
static size_t valid_cap   = 0;              /* allocated capacity        */

static uint32_t *all_primes = NULL;        /* all primes found so far   */
static size_t all_len      = 0;
static size_t all_cap      = 0;

/* Ensure we have at least n valid primes in the cache. */
static void ensure_valid_primes(idx_t n)
{
    if (valid_len >= n)
        return;

    /* initialise on first call */
    if (valid_cap == 0) {
        valid_cap   = 256;
        valid_primes = (seq_t*)malloc(valid_cap * sizeof(seq_t));
        all_cap     = 256;
        all_primes  = (uint32_t*)malloc(all_cap * sizeof(uint32_t));
    }

    /* start searching from the next integer after the last tested number */
    uint32_t candidate = (all_len == 0) ? 2 : all_primes[all_len-1] + 1;
    if (candidate == 2) {
        /* Fix to start from 2 */
    } else {
        /* Ensure we start with an odd number. */
        if ((candidate & 1) == 0)
            ++candidate;
    }

    while (valid_len < n) {
        /* Check if candidate is prime */
        bool prime = is_prime_uint32(candidate, all_primes, all_len);

        if (prime) {
            /* store in all primes list */
            if (all_len == all_cap) {
                all_cap *= 2;
                all_primes = (uint32_t*)realloc(all_primes, all_cap * sizeof(uint32_t));
            }
            all_primes[all_len++] = candidate;

            /* check residue condition: 1,3,5,7 mod 18 */
            uint32_t r = candidate % 18U;
            if (r == 1U || r == 3U || r == 5U || r == 7U) {
                if (valid_len == valid_cap) {
                    valid_cap *= 2;
                    valid_primes = (seq_t*)realloc(valid_primes, valid_cap * sizeof(seq_t));
                }
                valid_primes[valid_len++] = (seq_t)candidate;
            }
        }
        /* move to next candidate */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* undefined for 0, return 0 for safety */

    ensure_valid_primes(n);
    return valid_primes[n-1];
}
