/*
 * A064451  LCM of totients of binomial coefficients C(n,j), j = 0..n.
 * https://oeis.org/A064451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 29, u64 n <= 57, u128 n <= 111,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A064451: a(n) = lcm of φ( C(n,j) ) for all j=0…n */

// Required headers and utility: gcd provides seq_t type compatibility.
// No other helpers are needed.
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"      /* shows seq_t, seq_t gcd(seq_t, seq_t) */

/* Helper structure for the factorisation of (p-1) */
typedef struct {
    int pi;   /* index of the prime divisor in primes[] */
    unsigned exp;
} pminus1_factor;

/* Static tables ----------------------------------------------------------- */
static int *primes = NULL;           /* primes up to current_max_n */
static size_t num_primes = 0;
static int *prime_idx = NULL;        /* prime_idx[p] = index in primes or -1 */
static uint16_t *factExp = NULL;     /* factorial prime exponents */
static pminus1_factor **pminus1Facts = NULL;
static size_t *pminus1Len = NULL;
static size_t current_max_n = 0;

/* ------------------------------------------------------------------------ */
static void free_tables(void) {
    free(primes);
    free(prime_idx);
    free(factExp);
    for (size_t i = 0; i < num_primes; ++i) free(pminus1Facts[i]);
    free(pminus1Facts);
    free(pminus1Len);
    primes = NULL;
    prime_idx = NULL;
    factExp = NULL;
    pminus1Facts = NULL;
    pminus1Len = NULL;
    num_primes = 0;
    current_max_n = 0;
}

/* Initialise tables up to a given n */
static void init_tables(idx_t n) {
    if (current_max_n >= (size_t)n) return;   /* already enough */

    free_tables();                           /* release old data */

    size_t N = (size_t)n + 1;                /* factorial up to n */

    /* Smallest prime factor sieve up to n */
    int *spf = (int *)calloc(N, sizeof(int));
    prime_idx = (int *)calloc(N, sizeof(int));
    for (size_t i = 0; i < N; ++i) prime_idx[i] = -1;

    for (size_t i = 2; i < N; ++i) {
        if (!spf[i]) {
            spf[i] = (int)i;
            for (size_t j = i * i; j < N; j += i)
                if (!spf[j]) spf[j] = (int)i;
        }
    }

    /* Extract primes and map index */
    for (size_t i = 2; i < N; ++i)
        if (spf[i] == (int)i) {
            primes = (int *)realloc(primes, (num_primes + 1) * sizeof(int));
            primes[num_primes] = (int)i;
            prime_idx[i] = (int)num_primes;
            ++num_primes;
        }

    /* Allocate factorial exponent table */
    factExp = (uint16_t *)calloc(num_primes * N, sizeof(uint16_t));

    /* Build factorial exponents: factExp[pi*N + k] */
    for (size_t k = 1; k < N; ++k) {
        for (size_t pi = 0; pi < num_primes; ++pi)
            factExp[pi * N + k] = factExp[pi * N + (k - 1)];

        size_t tmp = k;
        while (tmp > 1) {
            int p = spf[tmp];
            uint16_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            int pi = prime_idx[p];
            factExp[pi * N + k] += cnt;
        }
    }

    /* Factorisations of (p-1) for each prime */
    pminus1Facts = (pminus1_factor **)malloc(num_primes * sizeof(pminus1_factor *));
    pminus1Len = (size_t *)calloc(num_primes, sizeof(size_t));

    for (size_t pi = 0; pi < num_primes; ++pi) {
        int p = primes[pi];
        size_t m = (size_t)(p - 1);
        if (m <= 1) {
            pminus1Facts[pi] = NULL;
            pminus1Len[pi] = 0;
            continue;
        }
        pminus1_factor *arr = NULL;
        size_t len = 0;
        while (m > 1) {
            int q = spf[m];
            unsigned cnt = 0;
            while (m % q == 0) {
                m /= q;
                ++cnt;
            }
            int qpi = prime_idx[q];
            arr = (pminus1_factor *)realloc(arr, (len + 1) * sizeof(pminus1_factor));
            arr[len].pi = qpi;
            arr[len].exp = cnt;
            ++len;
        }
        pminus1Facts[pi] = arr;
        pminus1Len[pi] = len;
    }

    free(spf);
    current_max_n = n;
}

/* ------------------------------------------------------------------------ */
/* Power by squaring using the standard * operator on seq_t */
static seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Sequence function ------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* offset is 1, safety for n=0 */

    if (n > current_max_n) init_tables(n);

    size_t N = (size_t)n + 1;
    uint32_t *maxExp = (uint32_t *)calloc(num_primes, sizeof(uint32_t));
    uint32_t *curExp = (uint32_t *)calloc(num_primes, sizeof(uint32_t));

    for (idx_t j = 1; j < n; ++j) {           /* j=0 and j=n give φ=1 */
        memset(curExp, 0, num_primes * sizeof(uint32_t));

        for (size_t pi = 0; pi < num_primes; ++pi) {
            uint16_t e = factExp[pi * N + n] -
                         factExp[pi * N + j] -
                         factExp[pi * N + (n - j)];
            if (e == 0) continue;

            curExp[pi] += (uint32_t)(e - 1);  /* p^(e-1) part of φ */

            /* multiply by (p-1) once */
            size_t len = pminus1Len[pi];
            pminus1_factor *arr = pminus1Facts[pi];
            for (size_t fi = 0; fi < len; ++fi) {
                int qpi = arr[fi].pi;
                curExp[qpi] += arr[fi].exp;
            }
        }

        for (size_t pi = 0; pi < num_primes; ++pi) {
            if (curExp[pi] > maxExp[pi]) maxExp[pi] = curExp[pi];
        }
    }

    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < num_primes; ++pi) {
        uint32_t exp = maxExp[pi];
        if (exp == 0) continue;
        seq_t base = (seq_t)primes[pi];
        seq_t pow = power_seq(base, exp);
        result = result * pow;
    }

    free(maxExp);
    free(curExp);
    return result;
}
