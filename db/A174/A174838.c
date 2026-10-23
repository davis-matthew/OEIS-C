/*
 * A174838  Numbers k such that semiprime(k)+1 is prime, where semiprime(k) is
 *          A001358.
 * https://oeis.org/A174838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that semiprime(k)+1 is prime
 *            (semiprime(k) is the k-th semiprime, i.e. the k-th number that
 *            is a product of exactly two primes)
 * Range:     u8 n <= 12, u16 n <= 601, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

#define LIMIT 5000000U          /* upper bound for numbers whose prime
                                 factors we need to know */
#define MAX_SEMIPRIMES 1000000U /* pre‑allocate space for semiprimes */

/* -------------------------------------------------------------
   Automatic initialization (sieved SPF and semiprime list)
   ------------------------------------------------------------- */
static uint32_t *spf = NULL;                /* smallest prime factor */
static seq_t    *semiprimes = NULL;         /* list of semiprimes */
static size_t    semiprime_count = 0;        /* how many we have kept */

/* Cast idx_t to size_t safely for array indexing   */
static inline size_t to_sz(idx_t idx) { return (size_t)idx; }

/* Precompute SPF up to LIMIT and build the semiprime list. */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* --- SPF sieve --------------------------------------------- */
    spf = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime               */
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* --- Build semiprime list --------------------------------- */
    semiprimes = (seq_t *)malloc((size_t)MAX_SEMIPRIMES * sizeof(seq_t));
    if (!semiprimes) exit(EXIT_FAILURE);

    for (uint32_t x = 4; x <= LIMIT && semiprime_count < MAX_SEMIPRIMES; ++x) {
        /* count prime factors with multiplicity */
        uint32_t t   = x;
        uint8_t  pfcnt = 0;
        while (t > 1 && pfcnt <= 2) {
            uint32_t p = spf[t];
            while (t % p == 0) {
                t   /= p;
                pfcnt++;
                if (pfcnt > 2) break;
            }
        }
        if (pfcnt == 2) semiprimes[semiprime_count++] = (seq_t)x;
    }
}

/* -------------------------------------------------------------
   Array of found qualifying indices (k values)
   ------------------------------------------------------------- */
static seq_t *qual     = NULL; /* array of qualifying k's */
static size_t qual_size = 0;   /* number stored so far   */
static size_t qual_cap  = 0;   /* allocated capacity      */
static idx_t  next_k    = 1;   /* smallest k not yet tested */

/* Helper: ensure qual array has room for another element. */
static void add_qual(seq_t val)
{
    if (qual_size == qual_cap) {
        size_t new_cap = (qual_cap == 0) ? 16 : qual_cap * 2;
        seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        qual = tmp;
        qual_cap = new_cap;
    }
    qual[qual_size++] = val;
}

/* -------------------------------------------------------------
   Main routine
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();

    /* grow the list of qualifying numbers until we have n of them */
    while (qual_size < n) {
        if (to_sz(next_k) > semiprime_count) {
            /* Should not happen with the chosen LIMIT. */
            exit(EXIT_FAILURE);
        }

        seq_t sem_val = semiprimes[to_sz(next_k) - 1];
        seq_t cand    = sem_val + 1ULL;

        if (is_prime_small(cand))
            add_qual((seq_t)next_k);

        ++next_k;
    }

    return qual[to_sz(n) - 1];
}
