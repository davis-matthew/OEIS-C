/*
 * A088700  Number of primes between successive semiprimes.
 * https://oeis.org/A088700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes strictly between the nth and (n+1)th
 *            semiprimes
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes, cmp_seq_t, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/cmp_seq_t.h"     /* int cmp_seq_t(const void *a, const void *b); */
#include "../utils/seq_alloc.h"     /* seq_t *seq_alloc(size_t n); void seq_free_array(seq_t *p); */

/* Types supplied by the build */
extern seq_t *seq_alloc(size_t);
extern void seq_free_array(seq_t *);

static bool initialized = false;

/* Prime list */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Prefix sum of the prime counting function */
static seq_t *pi_arr = NULL;       /* pi_arr[i] = number of primes <= i, for i <= SIEVE_LIMIT */
static size_t pi_len = 0;          /* = SIEVE_LIMIT + 1 */

/* Semiprime list (sorted) */
static seq_t *semiprimes = NULL;
static size_t semiprime_cnt = 0;
static size_t semiprime_cap = 0;

/* Upper bound for sieve and semiprime generation.
   500 000 comfortably covers the first 10 000 semiprimes. */
#define SIEVE_LIMIT 500000

/* Allocate and initialize all data structures */
static void init(void)
{
    if (initialized)
        return;

    /* 1. Generate all primes up to SIEVE_LIMIT. */
    build_primes(SIEVE_LIMIT, &primes, &prime_cnt);

    /* 2. Build pi_arr[0..SIEVE_LIMIT] */
    pi_len = SIEVE_LIMIT + 1;
    pi_arr  = seq_alloc(pi_len);   /* ptr to an array of seq_t */
    size_t count = 0;
    size_t pi_idx = 0;
    for (size_t i = 0; i < pi_len; ++i) {
        while (pi_idx < prime_cnt && primes[pi_idx] == i) {
            ++count;
            ++pi_idx;
        }
        pi_arr[i] = (seq_t)count;
    }

    /* 3. Generate all semiprimes <= SIEVE_LIMIT (p*q, p <= q). */
    semiprime_cap = 300000;          /* generous capacity for 10k+ semiprimes */
    semiprimes = seq_alloc(semiprime_cap);
    semiprime_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t prod = primes[i] * primes[j];
            if (prod > SIEVE_LIMIT)
                break;
            if (semiprime_cnt == semiprime_cap)
                abort();          /* should never happen with chosen capacity */
            semiprimes[semiprime_cnt++] = (seq_t)prod;
        }
    }

    /* 4. Sort the semiprimes into strictly increasing order. */
    qsort(semiprimes, semiprime_cnt, sizeof(seq_t), cmp_seq_t);

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n == 0)
        return (seq_t)0;            /* offset is 1; behaviour for n<1 is irrelevant */

    /* Convert to 0‑based indices for the semiprime array. */
    size_t i = (size_t)(n - 1);
    size_t j = i + 1;

    if (j >= semiprime_cnt)        /* safety check; should not happen for n ≤ 10000 */
        abort();

    seq_t m1 = semiprimes[i];
    seq_t m2 = semiprimes[j];

    /* Count primes strictly between m1 and m2:
       pi(m2-1) - pi(m1)  */
    seq_t pi_m1   = pi_arr[(size_t)m1];
    seq_t pi_m2_1 = pi_arr[(size_t)(m2 - 1)];

    return pi_m2_1 - pi_m1;
}
