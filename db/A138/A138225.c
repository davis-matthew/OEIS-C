/*
 * A138225  Number of pairs (p,q) of primes with p < q < prime(n), p + q >
 *          prime(n), q + prime(n) > p and prime(n) + p > q.
 * https://oeis.org/A138225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime pairs (p,q) with p < q < prime(n), p + q
 *            > prime(n) (the other two triangle inequalities are automatic)
 * Range:     u8 n <= 38, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/sieve_primes.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Cached primes up to the current sieve limit. */
static idx_t *prime_cache   = NULL;  /* array of primes <= current limit */
static size_t prime_cnt     = 0;      /* number of primes in the array   */
static idx_t  prime_limit   = 0;      /* current sieve limit             */

/* Rebuild the sieve cache up to 'limit' (inclusive). */
static void rebuild_cache(idx_t limit)
{
    if (prime_cache) {
        free(prime_cache);
        prime_cache   = NULL;
        prime_cnt     = 0;
        prime_limit   = 0;
    }

    if (limit < 2) {
        return;                     /* no primes to cache */
    }

    /* sieve_primes allocates memory for the array internally. */
    sieve_primes(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

/* Count pairs (p_i , p_j) among primes < p_n that satisfy p_i + p_j > p_n. */
static seq_t count_pairs(idx_t p_n)
{
    /* Find the number of primes strictly less than p_n. */
    size_t m = 0;
    while (m < prime_cnt && prime_cache[m] < p_n) {
        ++m;
    }
    if (m < 2) {
        return (seq_t)0;
    }

    seq_t res = (seq_t)0;
    size_t i = 0;
    size_t j = m - 1;

    while (i < j) {
        idx_t sum = prime_cache[i] + prime_cache[j];
        if (sum > p_n) {
            /* All pairs (i, k) for k = i+1 .. j satisfy the condition. */
            res += (seq_t)(j - i);
            j--;          /* move the upper index left for next iteration */
        } else {
            i++;          /* sum too small, need a larger lower index */
        }
    }
    return res;
}

/* Main entry function required by the testcase harness */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    idx_t p_n = get_prime(n);          /* n-th prime */

    /* Rebuild cache if we haven't sieved far enough yet */
    if (p_n > prime_limit) {
        rebuild_cache(p_n);
    }

    return count_pairs(p_n);
}
