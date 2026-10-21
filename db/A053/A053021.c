/*
 * A053021  Number of divisors function applied twice to n!.
 * https://oeis.org/A053021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 143, u32 n <= 846, u64 n <= 5657,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A053021: a(n) = d(d(n!))
 * Compute τ(τ(n!)) where τ(x) is the number of positive divisors of x.
 * Algorithm:
 *   1. Compute for each prime p ≤ n its exponent e_p in n! (Legendre formula).
 *   2. Let X_p = e_p + 1. The product P = ∏_p X_p equals τ(n!).
 *   3. Factor each X_p and accumulate the exponents of all primes in P.
 *   4. The result is τ(P) = ∏_q (f_q + 1) where f_q is the total exponent of q in P.
 *
 * All arithmetic is performed using the build‑specific integer type seq_t.
 * The routine is thread‑safe only in one thread; all static data are
 * lazily initialised and reused between calls.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* ----------  Prime sieve utilities  ---------- */
static size_t  sieve_limit  = 0;        /* current maximum n for which primes are known      */
static int    *prime_index  = NULL;      /* mapping prime → index in primes array */
static size_t  prime_count  = 0;        /* number of primes ≤ sieve_limit                 */
static size_t  primes_cap   = 0;        /* allocated capacity of primes array              */
static idx_t  *primes       = NULL;      /* array of primes up to sieve_limit               */
static uint8_t *composite   = NULL;     /* sieve table (0 = prime, 1 = composite)         */
static size_t  composite_cap= 0;        /* allocated size of composite (≥ sieve_limit+1) */

/* Recompute primes up to at least 'limit' (inclusive). */
static void ensure_primes(idx_t limit)
{
    if (limit <= (idx_t)sieve_limit)
        return;  /* already computed */

    /* Extend composite array if needed */
    size_t new_cap = (size_t)limit + 1;
    if (new_cap > composite_cap) {
        composite = (uint8_t *)realloc(composite, new_cap);
        if (!composite) abort();
        /* initialise new entries to 0 (prime) */
        memset(composite, 0, new_cap);
        composite_cap = new_cap;
    } else {
        memset(composite, 0, new_cap);      /* reset whole table for simplicity */
    }

    /* Simple Eratosthenes sieve up to 'limit' */
    size_t sqrt_lim = (size_t)floor(sqrt((double)limit)) + 1;
    for (size_t p = 2; p <= sqrt_lim; ++p) {
        if (!composite[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                composite[m] = 1;
        }
    }

    /* Allocate (or enlarge) primes array */
    size_t est_primes = limit / 10 + 10;   /* upper bound for π(limit) */
    if (est_primes > primes_cap) {
        primes_cap = est_primes;
        primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
        if (!primes) abort();
    }

    /* Allocate (or enlarge) prime_index mapping */
    size_t map_size = (size_t)limit + 1;
    prime_index = (int *)realloc(prime_index, map_size * sizeof(int));
    if (!prime_index) abort();
    memset(prime_index, -1, map_size * sizeof(int));

    /* Fill primes array and prime_index */
    prime_count = 0;
    for (size_t n = 2; n <= (size_t)limit; ++n) {
        if (!composite[n]) {
            if (prime_count == primes_cap) {          /* very unlikely for our limits */
                primes_cap *= 2;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) abort();
            }
            primes[prime_count] = (idx_t)n;
            prime_index[n] = (int)prime_count;
            ++prime_count;
        }
    }

    sieve_limit = limit;
}

/* ----------  Main sequence function  ---------- */
static uint64_t *freq   = NULL;   /* array of exponents of primes in P = τ(n!)   */
static size_t   freq_cap= 0;     /* current capacity of freq array             */

/* Legendre exponent of prime p in n! */
static inline uint64_t legendre_exp(idx_t n, idx_t p)
{
    uint64_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n = 0, but harmless */

    /* Ensure we have all primes up to n. */
    ensure_primes(n);

    /* Prepare frequency array for the factorisation of P = τ(n!). */
    if (freq_cap < prime_count) {
        freq = (uint64_t *)realloc(freq, prime_count * sizeof(uint64_t));
        if (!freq) abort();
        freq_cap = prime_count;
    }
    memset(freq, 0, prime_count * sizeof(uint64_t));

    /* For each prime p ≤ n compute e_p + 1 and factor it. */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];

        /* exponent of p in n! */
        uint64_t e = legendre_exp(n, p);

        /* X_p = e + 1 */
        idx_t X = (idx_t)(e + 1);

        /* Factor X by trial division over our prime list */
        idx_t tmp = X;
        for (size_t j = 0; j < prime_count && (idx_t)primes[j] * (idx_t)primes[j] <= tmp; ++j) {
            idx_t q = primes[j];
            if (tmp % q == 0) {
                uint64_t cnt = 0;
                while (tmp % q == 0) {
                    tmp /= q;
                    ++cnt;
                }
                freq[j] += cnt;
            }
        }
        if (tmp > 1) {                 /* remaining prime factor */
            int idx = prime_index[(size_t)tmp];
            if (idx >= 0)
                freq[idx] += 1;
        }
    }

    /* Compute τ(P) = product of (exponent + 1) over all primes in P. */
    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        if (freq[i] != 0) {
            result *= (seq_t)(freq[i] + 1);
        }
    }
    return result;
}
