/*
 * A066901  Let phi(k) determine the x-coordinate and prime(k) the
 *          y-coordinate of the point A(k). Sequence gives values of n such
 *          that points A(n), A(n+1) and A(n+2) form a right triangle with
 *          right angle at A(n+1).
 * https://oeis.org/A066901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the n-th integer k for which the right triangle * formed
 *            by points A(k)=([phi(k),prime(k)]), A(k+1), A(k+2) has a right *
 *            angle at A(k+1). The implementation performs a linear search,
 *            keeping * the sequence memoised in static storage
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------- */
/*  Constants                                                */
/* ---------------------------------------------------------------------- */
#define MAX_K            3000000          /* maximum index k to be searched */
#define PRIME_LIMIT      50000000         /* limit for sieving primes          */
#define PRIME_ARRAY_SIZE 3005000          /* enough to hold the first 3M primes */
#define PRIME_SIEVE_MAX  300000          /* for linear sieve prime list      */

/* ---------------------------------------------------------------------- */
/*  Global arrays for phi and prime factorisation                         */
/* ---------------------------------------------------------------------- */
static uint32_t phi[MAX_K + 3];
static uint32_t spf[MAX_K + 3];
static uint32_t primes_spf[PRIME_SIEVE_MAX];
static size_t   primes_spf_cnt = 0;

/* ---------------------------------------------------------------------- */
/*  Global arrays for kth primes                                          */
/* ---------------------------------------------------------------------- */
static uint8_t  *isPrime = NULL;               /* sieve up to PRIME_LIMIT       */
static uint32_t  primes[PRIME_ARRAY_SIZE];     /* first PRIME_ARRAY_SIZE primes   */
static size_t    prime_cnt = 0;

/* ---------------------------------------------------------------------- */
/*  Storage for the sequence itself                                       */
/* ---------------------------------------------------------------------- */
static uint32_t *seq_terms = NULL;
static size_t   seq_terms_len = 0, seq_terms_cap = 0;

/* ---------------------------------------------------------------------- */
/*  State of initialization                                               */
/* ---------------------------------------------------------------------- */
static int initialized = 0;
static uint32_t processed_k = 1;   /* next k to be examined */

/* ---------------------------------------------------------------------- */
/*  Append a new term to seq_terms                                         */
/* ---------------------------------------------------------------------- */
static void append_term(uint32_t val) {
    if (seq_terms_len == seq_terms_cap) {
        size_t new_cap = seq_terms_cap ? seq_terms_cap * 2 : 32;
        uint32_t *tmp = (uint32_t *)realloc(seq_terms, new_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_terms = tmp;
        seq_terms_cap = new_cap;
    }
    seq_terms[seq_terms_len++] = val;
}

/* ---------------------------------------------------------------------- */
/*  Pre‑compute phi, smallest prime factor array, and the prime table    */
/* ---------------------------------------------------------------------- */
static void init(void) {
    /* 1. Compute spf and phi up to MAX_K+2 using linear sieve          */
    phi[1] = 1;
    for (uint32_t i = 2; i <= MAX_K + 2; ++i) {
        if (spf[i] == 0) {                     /* i is prime                */
            spf[i] = i;
            phi[i] = i - 1;
            primes_spf[primes_spf_cnt++] = i;
        }
        for (size_t j = 0; j < primes_spf_cnt; ++j) {
            uint32_t p = primes_spf[j];
            uint64_t v = (uint64_t)i * p;
            if (v > MAX_K + 2) break;
            spf[(uint32_t)v] = p;
            if (p == spf[i]) {
                phi[(uint32_t)v] = phi[i] * p;
            } else {
                phi[(uint32_t)v] = phi[i] * (p - 1);
            }
            if (p == spf[i]) break;
        }
    }

    /* 2. Sieve primes up to PRIME_LIMIT                                 */
    isPrime = (uint8_t *)malloc(PRIME_LIMIT + 1);
    if (!isPrime) exit(EXIT_FAILURE);
    memset(isPrime, 1, PRIME_LIMIT + 1);
    isPrime[0] = isPrime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (isPrime[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                isPrime[j] = 0;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT && prime_cnt < PRIME_ARRAY_SIZE; ++i) {
        if (isPrime[i]) primes[prime_cnt++] = i;
    }
    if (prime_cnt < MAX_K + 3) {
        /* Not enough primes – shouldn't happen with the chosen limits   */
        exit(EXIT_FAILURE);
    }

    /* 3. Prepare the sequence storage                                   */
    seq_terms = NULL;
    seq_terms_len = 0;
    seq_terms_cap = 0;
    initialized = 1;
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (!initialized) init();

    /* Generate sequence until the required element is available       */
    while (seq_terms_len < n) {
        if (processed_k + 2 > MAX_K) break;    /* safety guard */

        int64_t dx1 = (int64_t)phi[processed_k]     - (int64_t)phi[processed_k + 1];
        int64_t dx2 = (int64_t)phi[processed_k + 1] - (int64_t)phi[processed_k + 2];
        int64_t dy1 = (int64_t)primes[processed_k - 1] - (int64_t)primes[processed_k];
        int64_t dy2 = (int64_t)primes[processed_k]   - (int64_t)primes[processed_k + 1];

        if (dx1 * dx2 + dy1 * dy2 == 0)
            append_term(processed_k);

        ++processed_k;
    }

    return (seq_t)seq_terms[n - 1];
}
