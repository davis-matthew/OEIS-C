/*
 * A063639  Primes of the form p*q*r - 1, where p, q and r are primes (not
 *          necessarily distinct).
 * https://oeis.org/A063639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063639 – primes of the form p*q*r – 1 (p, q, r primes, not
 * necessarily distinct).  The function generates all primes and keeps
 * them in an array, then tests each prime n to see whether n+1 factors
 * into exactly three primes (counted with multiplicity).  The first
 * n such primes are stored in a static cache, which is returned on
 * subsequent calls.  This approach runs in O(n) time for the requested
 * index and uses only tables of modest size. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- configuration ----------------------------------------------------- */
#define MAX_CACHE   20000   /* enough for n up to at least 10000 */
#define MAX_PRIMES  200000  /* primes stored for factoring */

static seq_t     cache[MAX_CACHE];
static size_t    cache_size = 0;

static uint64_t  primes[MAX_PRIMES];
static size_t    primes_size = 0;
static uint64_t  prime_candidate = 2;   /* next candidate to test */

/* ---- prime generation --------------------------------------------------- */
static uint64_t next_prime(void)
{
    while (1) {
        uint64_t cand = prime_candidate;
        bool is_prime = true;

        /* test divisibility only by primes ≤ sqrt(cand) */
        for (size_t i = 0; i < primes_size &&
                           (uint64_t)primes[i] * primes[i] <= cand; ++i)
        {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (primes_size == MAX_PRIMES)
                return 0;   /* safety – capacity exhausted */

            primes[primes_size++] = cand;
            prime_candidate = (cand == 2) ? 3 : cand + 2;
            return cand;
        }

        /* advance candidate (skip even numbers after 2) */
        prime_candidate = (cand == 2) ? 3 : cand + 2;
    }
}

/* Ensure that primes[] contains all primes whose square ≤ limit.       */
static void ensure_primes_upto(uint64_t limit)
{
    if (primes_size == 0)
        next_prime();  /* generate prime 2 */

    while ((uint64_t)primes[primes_size - 1] *
           primes[primes_size - 1] <= limit)
    {
        if (primes_size == MAX_PRIMES)   /* safety */
            break;
        next_prime();
    }
}

/* ---- factorisation ----------------------------------------------------- */
static int count_three_prime_factors(uint64_t m)
{
    int cnt = 0;
    ensure_primes_upto(m);

    for (size_t i = 0; i < primes_size &&
                       (uint64_t)primes[i] * primes[i] <= m; ++i)
    {
        uint64_t p = primes[i];
        while (m % p == 0) {
            m /= p;
            if (++cnt > 3)
                return cnt;   /* early exit */
        }
    }
    if (m > 1)
        ++cnt;
    return cnt;
}

/* ---- main accessor ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of range – not used */

    /* generate primes and cache until the nth term is found */
    while (cache_size < (size_t)n) {
        uint64_t p = next_prime();   /* prime candidate */

        if (count_three_prime_factors(p + 1) == 3) {
            cache[cache_size++] = (seq_t)p;
        }
    }
    return cache[(size_t)n - 1];
}
