/*
 * A093772  a(n) is the smallest integer at which the value of the "truncated
 *          Mertens function" (= A088004) equals n.
 * https://oeis.org/A093772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that ∑_{k=1..m, k not prime} μ(k) = n
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define LIMIT_M 1000000U   /* upper bound for m (safe for n≤10000) */

static seq_t *first_occ = NULL;          /* first occurrence of each T value */
static size_t first_occ_cap = 0;
static bool *is_composite = NULL;        /* true if composite */
static signed char *mu = NULL;           /* Möbius function values */
static idx_t *primes = NULL;             /* list of primes (size <= LIMIT_M) */
static size_t prime_cnt = 0;
static bool initialized = false;

/* Build tables once */
static void init_sequence(void) {
    if (initialized) return;
    size_t limit = LIMIT_M;
    /* allocate arrays */
    is_composite = (bool*)malloc((limit + 1) * sizeof(bool));
    mu = (signed char*)malloc((limit + 1) * sizeof(signed char));
    primes = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    first_occ = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    first_occ_cap = limit + 1;

    if (!is_composite || !mu || !primes || !first_occ) return; /* out of memory */

    /* linear sieve to compute primes and Möbius */
    mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {          /* i is prime */
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes[j];
            idx_t t = i * p;
            if (t > limit) break;
            is_composite[t] = true;      /* composite */
            if (i % p == 0) {
                mu[t] = 0;
                break;
            } else {
                mu[t] = (signed char)(-mu[i]);
            }
        }
    }

    /* compute truncated Mertens function T(m) and record first occurrences */
    long long T_cur = 0;                  /* allow negative values */
    for (idx_t m = 1; m <= limit; ++m) {
        long long add = 0;
        if (m == 1) {
            add = mu[1];                 /* μ(1)=1, 1 is nonprime */
        } else if (!is_composite[m]) {
            add = 0;                     /* prime numbers are excluded */
        } else {
            add = mu[m];
        }
        T_cur += add;
        if (T_cur > 0 && (size_t)T_cur < first_occ_cap && first_occ[(size_t)T_cur] == 0) {
            first_occ[(size_t)T_cur] = (seq_t)m;
        }
    }

    initialized = true;
}

seq_t a(idx_t n) {
    init_sequence();                     /* ensure tables built */
    if (n == 0) return 0;                /* not used by sequence indexing */
    if (n >= first_occ_cap) return 0;    /* out of precomputed range */
    return first_occ[n];
}
