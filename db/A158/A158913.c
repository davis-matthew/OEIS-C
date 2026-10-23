/*
 * A158913  Primes p such that there is a composite c with sigma(p) =
 *          sigma(c).
 * https://oeis.org/A158913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence of primes p such that there is a composite c with σ(p) = σ(c).
   σ(n) = sum of divisors of n.
   Since σ(p) = p + 1 for prime p, the condition is that p + 1 occurs as σ(c)
   for some composite c. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>   /* uint8_t */

static const size_t LIMIT = 2000000;          /* upper bound for computing */
static bool           initialized = false;   /* has data been prepared? */

/* static storage for data */
static uint8_t  *is_prime   = NULL;   /* 0 or 1 flags for primality   */
static int      *prime_list = NULL;   /* list of all primes up to LIMIT   */
static size_t   prime_cnt   = 0;
static seq_t   *sigma       = NULL;   /* σ values for 0..LIMIT       */
static uint8_t  *reachable   = NULL;   /* reachable[σ] == 1 if some composite has this σ */
static int      *matches     = NULL;   /* primes satisfying the condition */
static size_t   match_cnt   = 0;

/* prepare all data once */
static void initialize(void)
{
    if (initialized) return;
    initialized = true;

    /* ---------- sieve of Eratosthenes ---------- */
    is_prime = (uint8_t*)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime) abort();
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) if (is_prime[i]) {
        for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
    }

    /* collect primes */
    size_t temp_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++temp_cnt;
    prime_list = (int*)malloc(temp_cnt * sizeof(int));
    if (!prime_list) abort();
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) prime_list[prime_cnt++] = (int)i;

    /* ---------- compute sigma values ---------- */
    sigma = (seq_t*)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!sigma) abort();
    memset(sigma, 0, (LIMIT + 1) * sizeof(seq_t));
    for (size_t i = 1; i <= LIMIT; ++i) {
        for (size_t j = i; j <= LIMIT; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* ---------- find maximal σ to size reachable array ---------- */
    seq_t max_sigma = 0;
    for (size_t i = 1; i <= LIMIT; ++i)
        if (sigma[i] > max_sigma) max_sigma = sigma[i];
    /* plus one to allow index = max_sigma */
    reachable = (uint8_t*)malloc(((size_t)max_sigma + 2) * sizeof(uint8_t));
    if (!reachable) abort();
    memset(reachable, 0, ((size_t)max_sigma + 2) * sizeof(uint8_t));

    /* mark reachable σ values from composites only */
    for (size_t i = 4; i <= LIMIT; ++i) if (!is_prime[i]) {
        seq_t s = sigma[i];
        size_t idx = (size_t)s;        /* safe, σ values are modest */
        reachable[idx] = 1;
    }

    /* ---------- collect primes satisfying the property ---------- */
    /* start with a modest capacity and grow as needed */
    size_t cap = 1024;
    matches = (int*)malloc(cap * sizeof(int));
    if (!matches) abort();
    match_cnt = 0;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        int p = prime_list[idx];
        seq_t target = (seq_t)p + 1ULL;   /* σ(p) */
        size_t tgt = (size_t)target;
        if (tgt < ((size_t)max_sigma + 2) && reachable[tgt]) {
            if (match_cnt == cap) {
                cap <<= 1;
                matches = (int*)realloc(matches, cap * sizeof(int));
                if (!matches) abort();
            }
            matches[match_cnt++] = p;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    initialize();

    if ((idx_t)match_cnt < n) {
        /* The precomputed limit was too small; in practice this should
           not happen for the requested range (up to 10000th term). */
        abort();
    }
    return (seq_t)matches[n - 1];
}
