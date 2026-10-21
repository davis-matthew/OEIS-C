/*
 * A065384  Largest prime <= n * (n + 1) / 2.
 * https://oeis.org/A065384
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime ≤ n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a dynamic sieve of primes up to the
   triangular number t = n*(n+1)/2.  The sieve is extended only as
   needed; primes are stored in a growing array.  For each call of a(),
   we ensure the sieve covers t and then search downward for the prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>   /* sqrt */

/* ---------- globals for the dynamic sieve ---------- */
static unsigned char *sieve     = NULL;   /* 0 = composite, 1 = prime */
static idx_t        sieve_limit = 0;     /* largest index currently sieved */
static uint64_t    *primes     = NULL;   /* list of primes found so far   */
static size_t      primes_cnt  = 0;      /* number of primes stored       */
static size_t      primes_cap  = 0;      /* capacity of primes array      */

/* Grow the primes array to hold at least one more prime. */
static void prime_grow(void)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? 2 * primes_cap : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Extend the sieve up to new_limit (inclusive).  Calls are idempotent. */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;              /* nothing to do */

    /* Allocate or grow the sieve array. */
    unsigned char *tmp = (unsigned char *)realloc(sieve, (size_t)(new_limit + 1));
    if (!tmp) abort();
    sieve = tmp;

    /* Initialize new entries to 'prime' (1).  We will correct 0 and 1 later. */
    for (idx_t i = sieve_limit + 1; i <= new_limit; ++i)
        sieve[i] = 1;

    /* Mark composites in the new range using primes already known. */
    idx_t old_limit = sieve_limit;
    idx_t limit     = new_limit;
    idx_t sqrt_limit = (idx_t)floor(sqrt((double)limit));

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        if ((idx_t)p > sqrt_limit) break;

        idx_t start = ((old_limit + 1 + p - 1) / p) * p;   /* first multiple */
        if (start < p * p) start = p * p;
        for (idx_t k = start; k <= limit; k += p)
            sieve[k] = 0;
    }

    /* Find new primes in the newly added segment and mark their multiples. */
    for (idx_t i = old_limit + 1; i <= limit; ++i) {
        if (i < 2) { sieve[i] = 0; continue; }          /* 0 and 1 are composite */
        if (sieve[i]) {                                 /* prime detected */
            prime_grow();
            primes[primes_cnt++] = (uint64_t)i;         /* store the prime */

            if (i <= sqrt_limit) {
                idx_t start = ((old_limit + 1 + i - 1) / i) * i;
                if (start < i * i) start = i * i;
                for (idx_t k = start; k <= limit; k += i)
                    sieve[k] = 0;
            }
        }
    }

    /* Ensure 0 and 1 remain composite, even if they were in the first segment. */
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    sieve_limit = limit;
}

/* ---------------------------------------------------- */

/* Main function to compute the sequence value.
   Defined for n ≥ 2; behaviour for lower n is irrelevant. */
seq_t a(idx_t n)
{
    /* Compute the triangular number t = n*(n+1)/2 as seq_t. */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* We only support t that fit into idx_t for the sieve. */
    if (t > (seq_t)sieve_limit) {
        /* uint64_t limit for sieve; larger values cannot be managed. */
        if (t > (seq_t)UINT64_MAX) abort();
        extend_sieve((idx_t)t);
    }

    /* Search downward for the largest prime ≤ t. */
    idx_t x = (idx_t)t;
    while (x >= 2) {
        if (sieve[x]) return (seq_t)x;
        --x;
    }

    /* Fallback – should never happen for n ≥ 2. */
    return (seq_t)2;
}
