/*
 * A073770  Primes p not of the form q + s where q is prime and s > 0 is the
 *          smallest square such that q + s is prime.
 * https://oeis.org/A073770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p that cannot be written as p = q + s
 *            where q is prime, s>0 is the smallest square such that q+s is
 *            prime
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: we keep a dynamically growing list of primes and an array
   marking the primes excluded by the rule.  When more terms are required
   we extend the sieve, recompute the exclusions and rebuild the sequence. */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* Global tables used by the computation */
static seq_t *seqA          = NULL;     /* sequence values                     */
static size_t seqA_len      = 0;         /* number of computed terms            */
static idx_t  current_limit = 0;        /* last prime limit processed          */
static bool   *is_prime     = NULL;     /* sieve table                         */
static uint32_t *primes    = NULL;     /* list of primes up to current_limit  */
static size_t   primes_count   = 0;     /* number of primes in the list        */

/* --------------------------------------------------------------------- */
/* Helper: compute primes up to n using a standard sieve (array of bool) */
static void compute_primes(idx_t n)
{
    /* allocate or reallocate sieve array */
    if (is_prime == NULL) {
        is_prime = (bool *)malloc((size_t)(n + 1));
    } else {
        is_prime = (bool *)realloc(is_prime, (size_t)(n + 1));
    }
    /* initialise: all true; 0 and 1 are not prime */
    for (size_t i = 0; i <= (size_t)n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    idx_t sq = (idx_t)sqrt((double)n);
    for (idx_t i = 2; i <= sq; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[(size_t)j] = false;
            }
        }
    }

    /* build list of primes */
    /* allocate at most n/log n primes – rough upper bound */
    size_t est = (n >= 2) ? (size_t)(n / log((double)n) * 1.2) : 2;
    if (primes == NULL) {
        primes = (uint32_t *)malloc(est * sizeof(uint32_t));
    } else {
        primes = (uint32_t *)realloc(primes, est * sizeof(uint32_t));
    }
    primes_count = 0;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (is_prime[i]) {
            if (primes_count >= est) {          /* grow if necessary */
                est = est * 2 + 10;
                primes = (uint32_t *)realloc(primes, est * sizeof(uint32_t));
            }
            primes[primes_count++] = (uint32_t)i;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Rebuild sequence using primes up to new_limit (>= current_limit) */
static void recompute(idx_t new_limit)
{
    compute_primes(new_limit);

    /* mark excluded primes: for each prime q find smallest q + t^2 that is prime */
    bool *excluded = (bool *)malloc((size_t)(new_limit + 1));
    for (size_t i = 0; i <= (size_t)new_limit; ++i) excluded[i] = false;

    for (size_t pi = 0; pi < primes_count; ++pi) {
        uint32_t q = primes[pi];
        for (idx_t t = 1; ; ++t) {
            idx_t cand = (idx_t)q + t * t;
            if (cand > new_limit) break;
            if (is_prime[(size_t)cand]) {
                excluded[(size_t)cand] = true;
                break;
            }
        }
    }

    /* build new sequence table */
    seq_t *new_seqA = NULL;
    size_t new_len = 0;
    for (size_t pi = 0; pi < primes_count; ++pi) {
        uint32_t p = primes[pi];
        if (!excluded[(size_t)p]) {
            if (new_len >= seqA_len) {
                new_seqA = (seq_t *)realloc(new_seqA, (new_len + 1) * sizeof(seq_t));
                new_seqA[new_len] = (seq_t)p;
            } else {
                new_seqA[new_len] = (seq_t)p;
            }
            ++new_len;
        }
    }
    free(excluded);

    /* replace old sequence */
    free(seqA);
    seqA      = new_seqA;
    seqA_len  = new_len;
    current_limit = new_limit;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, avoid UB */

    /* ensure enough terms available */
    while (seqA_len < (size_t)n) {
        idx_t new_limit = current_limit ? current_limit * 2 : 100000;
        if (new_limit < n * 20) new_limit = n * 20;   /* heuristic bound */
        recompute(new_limit);
    }
    return seqA[(size_t)n - 1];
}
