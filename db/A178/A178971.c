/*
 * A178971  Indices of primes that are the sum of two Fibonacci numbers.
 * https://oeis.org/A178971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th prime that can be written as a sum of
 *            two Fibonacci numbers
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------- Fibonacci helpers -------------------------------- */
static uint64_t *fibs = NULL;        /* Fibonacci numbers, stored in uint64_t */
static size_t    fibs_cap = 0;
static size_t    fibs_len = 0;

static void grow_fibs(void)
{
    if (fibs_cap == 0) {
        fibs_cap = 64;
        fibs = (uint64_t *)malloc(fibs_cap * sizeof(uint64_t));
        fibs[0] = 0;          /* F0 */
        fibs[1] = 1;          /* F1 */
        fibs_len = 2;
    } else if (fibs_len == fibs_cap) {
        fibs_cap <<= 1;
        fibs = (uint64_t *)realloc(fibs, fibs_cap * sizeof(uint64_t));
    }
}

/* Ensure the Fibonacci array contains values up to at least max_val */
static void extend_fibs_up_to(uint64_t max_val)
{
    if (!fibs) grow_fibs();

    while (fibs_len >= 2 && fibs[fibs_len - 1] <= max_val) {
        /* grow if necessary */
        if (fibs_len == fibs_cap) grow_fibs();

        /* compute next Fibonacci */
        uint64_t next = fibs[fibs_len - 1] + fibs[fibs_len - 2];
        fibs[fibs_len++] = next;
    }
}

/* Binary search on fibs[] to test membership */
static bool is_fib(uint64_t v)
{
    size_t lo = 0, hi = fibs_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (fibs[mid] == v) return true;
        if (fibs[mid] < v)  lo = mid + 1;
        else                hi = mid;
    }
    return false;
}

/* --------------------- Prime helpers ----------------------------------- */
static uint64_t *primes = NULL;     /* list of primes found so far */
static size_t    primes_cap = 0;
static size_t    primes_len = 0;
static uint64_t  next_candidate = 2; /* next number to test for primality */

/* Grow dynamic prime buffer */
static void grow_primes(void)
{
    if (primes_cap == 0) {
        primes_cap = 1024;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    } else if (primes_len == primes_cap) {
        primes_cap <<= 1;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
}

/* Trial division primality test using previously found primes */
static bool is_prime_trial(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Return the next prime number (also appends it to the primes list) */
static uint64_t next_prime(void)
{
    while (true) {
        uint64_t cand = next_candidate;

        if (cand == 2) {
            next_candidate = 3;
            grow_primes();
            primes[primes_len++] = 2;
            return 2;
        }

        bool prime = is_prime_trial(cand);
        next_candidate = cand + 2;   /* skip even numbers */

        if (prime) {
            grow_primes();
            primes[primes_len++] = cand;
            return cand;
        }
    }
}

/* --------------------- Cache helpers ----------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* --------------------- Main function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, harmless */

    /* Make sure we have computed at least n terms */
    if (cache_len < (size_t)n) {
        /* Allocate/capacity for cache if needed */
        if (!cache) grow_cache();

        /* Generate terms until we have n of them */
        while (cache_len < (size_t)n) {
            uint64_t p = next_prime();            /* current prime */

            /* ensure Fibonacci list covers this prime */
            extend_fibs_up_to(p);

            /* check if p can be expressed as a sum of two Fibonacci numbers */
            bool ok = false;
            for (size_t i = 0; i < fibs_len && fibs[i] <= p/2; ++i) {
                uint64_t diff = p - fibs[i];
                if (is_fib(diff)) {
                    ok = true;
                    break;
                }
            }

            if (ok) {
                grow_cache();
                /* primes_len is the index (1‑based) of the current prime */
                cache[cache_len++] = (seq_t)primes_len;
            }
        }
    }

    return cache[n - 1];
}
