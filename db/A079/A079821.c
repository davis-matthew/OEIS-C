/*
 * A079821  Smallest n-digit multiple of the n-th prime.
 * https://oeis.org/A079821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = prime(n) * ceil(10^(n-1) / prime(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  = prime(n) * ((10^(n-1) + prime(n) - 1) / prime(n)) */

#include <stdbool.h>
#include <stdlib.h>

/*  seq_t and idx_t are supplied by the build environment.  */

/* ---------- Prime cache -------------------------------------------- */
static int *prime_cache    = NULL;  /* array of primes, 1‑indexed internally */
static size_t prime_len   = 0;      /* number of primes stored      */
static size_t prime_cap   = 0;      /* allocated capacity           */

/* Ensure that the prime cache holds at least 'need' primes.  */
static void ensure_primes(size_t need)
{
    if (need <= prime_len)
        return;  /* already sufficient */

    /* Grow the cache if necessary. */
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        int *tmp = (int *)realloc(prime_cache, new_cap * sizeof(int));
        if (!tmp) {
            /* Allocation failure – abort gracefully.  */
            exit(EXIT_FAILURE);
        }
        prime_cache = tmp;
        prime_cap   = new_cap;
    }

    /* Simple incremental prime generation using trial division.  */
    int candidate = (prime_len == 0) ? 2 : prime_cache[prime_len - 1] + 1;
    while (prime_len < need) {
        bool is_prime = true;
        /* Test divisibility by all known primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_len; ++i) {
            int p = prime_cache[i];
            /* Stop if p*p > candidate.  Use 64‑bit arithmetic to avoid overflow. */
            if ((idx_t)p * (idx_t)p > (idx_t)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_len++] = candidate;
        }
        ++candidate;
    }
}

/* Return the n‑th prime (1‑indexed).  */
static seq_t nth_prime(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not defined, but keep safe */
    ensure_primes((size_t)n);
    return (seq_t)prime_cache[n - 1];
}

/* ---------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* defined only for n ≥ 1 */

    /* 1. Retrieve the n‑th prime. */
    seq_t p = nth_prime(n);

    /* 2. Compute 10^(n-1) in a loop.  */
    seq_t base = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        base *= (seq_t)10;
    }

    /* 3. Compute ceil(base / p) = (base + p - 1) / p. */
    seq_t quotient = (base + p - (seq_t)1) / p;

    /* 4. Return the smallest n‑digit multiple of p. */
    return p * quotient;
}
