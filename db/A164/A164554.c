/*
 * A164554  Ramanujan primes A104272(n) for which A104272(n) = A080359(n).
 * https://oeis.org/A164554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A164554: Ramanujan primes A104272 that are
   equal to A080359.  The sequence consists of the primes p for which
   the value D(p) = pi(p) – pi(p/2) occurs only once among all primes.
   The function builds a sieve up to a fixed limit, computes the prefix
   prime counts and the D-values, and stores the primes with a unique
   D-value.  Subsequent calls return the nth element from the cached
   sequence.  (No external I/O; index 1 corresponds to the first term.
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* forward chunk for comparator used by qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* global static storage */
static uint8_t  *is_comp    = NULL;   /* composite marker cake */
static uint32_t *pi_arr     = NULL;   /* prefix prime counts */
static uint32_t *primes     = NULL;   /* list of primes */
static idx_t    primes_cnt  = 0;
static uint32_t *diff_count = NULL;   /* occurrence counter of D */
static uint32_t *diff_prime = NULL;   /* prime value for unique D */
static seq_t   *peculiar   = NULL;   /* final peculiar primes */
static size_t  pec_count   = 0;       /* number of peculiar primes */
static size_t  pec_cap     = 0;
static bool    precomputed = false;

/* precompute all necessary data */
static void precompute(void)
{
    if (precomputed) return;

    /* ------------------------------------------------------------
       Step 1: sieve primes up to a safe upper bound
       ------------------------------------------------------------ */
    const size_t LIMIT = 10000000ULL;          /* 10 000 000 → ≈664 000 primes */
    is_comp = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_comp) return;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    pi_arr = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!pi_arr) return;
    pi_arr[0] = 0;
    for (uint32_t i = 1; i <= LIMIT; ++i)
        pi_arr[i] = pi_arr[i - 1] + (!is_comp[i]);

    primes_cnt = pi_arr[LIMIT];
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) return;
    uint32_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            primes[idx++] = i;               /* store prime */

    /* ------------------------------------------------------------
       Step 2: compute D(p) = pi(p) - pi(p/2) for each prime,
       and count occurrences
       ------------------------------------------------------------ */
    uint32_t diff_max = pi_arr[LIMIT];      /* maximum possible D value */
    diff_count = (uint32_t *)calloc(diff_max + 1, sizeof(uint32_t));
    diff_prime = (uint32_t *)calloc(diff_max + 1, sizeof(uint32_t));
    if (!diff_count || !diff_prime) return;

    for (idx = 0; idx < primes_cnt; ++idx) {
        uint32_t p = primes[idx];
        uint32_t d = pi_arr[p] - pi_arr[p / 2];
        if (diff_count[d] == 0) {          /* first time seen */
            diff_count[d] = 1;
            diff_prime[d] = p;
        } else if (diff_count[d] == 1) {    /* second time → not unique */
            diff_count[d] = 2;
            diff_prime[d] = 0;
        }                                   /* further occurrences ignored */
    }

    /* ------------------------------------------------------------
       Step 3: collect primes with unique D into peculiar array
       ------------------------------------------------------------ */
    pec_cap = primes_cnt;                  /* upper bound for size */
#ifdef __cplusplus
    peculiar = new seq_t[pec_cap];
#else
    peculiar = (seq_t *)calloc(pec_cap, sizeof(seq_t));
#endif
    if (!peculiar) return;

    for (uint32_t d = 1; d <= diff_max; ++d) {
        if (diff_prime[d]) {                /* unique D */
            peculiar[pec_count++] = (seq_t)diff_prime[d];
        }
    }

    /* sort the sequence in increasing order */
    qsort(peculiar, pec_count, sizeof(seq_t), cmp_seq);

    precomputed = true;
}

/* ------------------------------------------------------------
   Public function as required by the specification.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!precomputed) precompute();
    if (n == 0) return (seq_t)0;
    size_t i = (size_t)(n - 1);                     /* 0‑based index */
    if (i >= pec_count) return (seq_t)0;           /* out of range */
    return peculiar[i];
}
