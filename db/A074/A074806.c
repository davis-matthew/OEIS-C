/*
 * A074806  Least k such that F(k) reduced (mod prime(n)) = prime(n)-1 where
 *          F(k) is the k-th Fibonacci number.
 * https://oeis.org/A074806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   seq_alloc_new_array, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A074806
 *
 *  a(n) = the least k such that F(k) ≡ prime(n)-1 (mod prime(n))
 *  where F(k) is the kth Fibonacci number (F(0)=0, F(1)=1).
 *
 *  The implementation pre‑computes the required nth primes on demand
 *  using the utility “sieve_primes” and memoises the sequence values.
 *  Fibonacci values are computed iteratively modulo the prime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/sieve_primes.h"
#include "../utils/seq_alloc_new_array.h"

/* ------------------------------------------------------------ */
/*  Helper: dynamic array of the first primes                    */

static uint64_t *primes = NULL;      /* array of primes (1‑based via index-1) */
static size_t   primes_cnt = 0;      /* number of primes stored          */
static size_t   primes_cap = 0;      /* capacity of primes array         */

/* Ensure that primes[0..idx-1] exist, generating more primes if needed */
static uint64_t get_prime(idx_t idx)
{
    /* A crude upper bound for the nth prime: n * 20 is ample for n < 10000 */
    if (primes_cnt < idx) {
        idx_t limit = idx * 20;
        if (limit < 1000) limit = 1000;            /* safety for tiny n */

        uint64_t *new_primes = NULL;
        size_t   new_cnt   = 0;
        sieve_primes(limit, &new_primes, &new_cnt);

        free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
        primes_cap = primes_cnt;
    }
    return primes[idx - 1];
}

/* ------------------------------------------------------------ */
/*  Memoised cache for sequence values                           */

static seq_t *cache   = NULL;      /* cache of a(n) values, 0‑initialised */
static size_t cache_cap = 0;       /* current capacity (>= 1) */

/* Ensure that cache can hold values up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_alloc_new_array(cache_cap);
        /* seq_alloc_new_array is expected to zero‑initialise memory */
    }
    while ((idx_t)cache_cap < n) {
        size_t old_cap = cache_cap;
        cache_cap <<= 1;
        seq_t *tmp = seq_alloc_new_array(cache_cap);
        memcpy(tmp, cache, old_cap * sizeof(seq_t));
        /* zero the newly added slots */
        memset(tmp + old_cap, 0, (cache_cap - old_cap) * sizeof(seq_t));
        free(cache);
        cache = tmp;
    }
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                        */

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n < 1, return 0 */

    /* Memoisation */
    ensure_cache(n);
    if (cache[n - 1] != 0) return cache[n - 1];

    uint64_t p      = get_prime(n);      /* nth prime            */
    uint64_t target = p - 1;             /* value to hit modulo p*/

    /* Special case: p == 2 yields a(1) = 1 directly */
    if (p == 2) {
        cache[n - 1] = (seq_t)1;
        return cache[n - 1];
    }

    /* Iteratively compute Fibonacci modulo p until we hit target */
    uint64_t f_prev = 0;   /* F(0) */
    uint64_t f_curr = 1;   /* F(1) */
    uint64_t k      = 1;   /* current index of F(k) */

    while (f_curr != target) {
        uint64_t f_next = f_prev + f_curr;
        if (f_next >= p) f_next -= p;      /* keep value < p */
        f_prev = f_curr;
        f_curr = f_next;
        ++k;
    }

    cache[n - 1] = (seq_t)k;
    return cache[n - 1];
}
