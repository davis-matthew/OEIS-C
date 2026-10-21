/*
 * A213520  Least number k0 such that (prime(k+1) / prime(k))^n < 2 for all k
 *          >= k0.
 * https://oeis.org/A213520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ceil_eps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least k0 such that (prime(k+1) / prime(k))^n < 2 for all k >= k0
 *           (equivalently, prime(k+1)/prime(k) < 2^(1/n))
 *
 * Computation:
 *   • Build a prime table up to a fixed limit (2 000 000).
 *   • For each consecutive pair compute n_min(k) = ceil(ln2 / ln(prime(k+1)/prime(k))).
 *     This is the least n for which the ratio satisfies the inequality.
 *   • For every n, the maximal k among ratios with n_min <= n gives the answer.
 *   • Store answers up to MAX_N and return them in O(1).  For larger n
 *     (unlikely in tests) fall back to a linear scan.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/ceil_eps.h"

/* --------------------------------------------------------------------- */
/* Configuration parameters                                            */
/* --------------------------------------------------------------------- */
#define LIMIT   2000000ULL          /* sieve limit for primes               */
#define MAX_N   200000U             /* maximum n pre‑computed                  */

/* --------------------------------------------------------------------- */
/* Helper cast for malloc in C++                                      */
/* --------------------------------------------------------------------- */
static inline void *safe_malloc(size_t size)
{
#ifdef __cplusplus
    return ::malloc(size);
#else
    return malloc(size);
#endif
}

/* --------------------------------------------------------------------- */
/* Global data                                                        */
/* --------------------------------------------------------------------- */
static uint32_t *g_primes = NULL;      /* array of primes (uint32)            */
static size_t   g_prime_cnt = 0;       /* number of primes                    */
static double  *g_log_prime = NULL;    /* ln(prime[i])                        */
static seq_t    g_a_table[MAX_N + 1];  /* pre‑computed a(n) values (offset 1) */
static bool     g_initialized = false; /* initialization flag                 */

/* --------------------------------------------------------------------- */
/* Utility: ceil with tiny epsilon to avoid rounding errors           */

/* --------------------------------------------------------------------- */
/* Build prime table up to LIMIT                                       */
/* --------------------------------------------------------------------- */
static void build_primes(void)
{
    const size_t n = (size_t)LIMIT;
    uint8_t *sieve = (uint8_t *)safe_malloc(n + 1);
    if (!sieve) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= n; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= n; ++i)
        if (sieve[i])
            for (uint32_t j = i * i; j <= n; j += i)
                sieve[j] = 0;

    /* Count primes */
    g_prime_cnt = 0;
    for (uint32_t i = 2; i <= n; ++i)
        if (sieve[i]) ++g_prime_cnt;

    g_primes = (uint32_t *)safe_malloc(g_prime_cnt * sizeof(uint32_t));
    if (!g_primes) exit(EXIT_FAILURE);
    size_t pos = 0;
    for (uint32_t i = 2; i <= n; ++i)
        if (sieve[i]) g_primes[pos++] = i;
    free(sieve);

    g_log_prime = (double *)safe_malloc(g_prime_cnt * sizeof(double));
    if (!g_log_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i < g_prime_cnt; ++i)
        g_log_prime[i] = log((double)g_primes[i]);

    /* Initialise a(1) */
    g_a_table[1] = (seq_t)1;
}

/* --------------------------------------------------------------------- */
/* Pre‑compute a(n) for 1 <= n <= MAX_N                                */
/* --------------------------------------------------------------------- */
static void precompute_a(void)
{
    /* best_k_of_n[n] = largest k whose n_min <= n */
    int32_t *best_k_of_n = (int32_t *)calloc(MAX_N + 1, sizeof(int32_t));
    if (!best_k_of_n) exit(EXIT_FAILURE);

    const double ln2 = log(2.0);

    for (size_t k = 0; k + 1 < g_prime_cnt; ++k) {
        double diff = g_log_prime[k + 1] - g_log_prime[k]; /* ln(ratio) */
        if (diff <= 0.0) continue;                        /* safety */
        double tmp = ln2 / diff;                          /* n_min */
        idx_t n_min = ceil_eps(tmp);
        if (n_min > MAX_N) continue;
        if ((int32_t)k > best_k_of_n[n_min])
            best_k_of_n[n_min] = (int32_t)k;
    }

    int32_t last_k = -1;
    for (uint32_t n = 2; n <= MAX_N; ++n) {
        if (best_k_of_n[n] > last_k)
            last_k = best_k_of_n[n];
        g_a_table[n] = (seq_t)((last_k == -1) ? 1 : ((uint64_t)last_k + 2));
    }

    free(best_k_of_n);
}

/* --------------------------------------------------------------------- */
/* Public function required by the problem                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!g_initialized) {
        build_primes();
        precompute_a();
        g_initialized = true;
    }

    if (n == 0) return (seq_t)0;      /* undefined, return 0 */
    if (n <= MAX_N) return g_a_table[n];

    /* Fallback for n > MAX_N: linear scan over primes */
    double threshold = exp(log(2.0) / (double)n); /* 2^(1/n) */
    int32_t last_k = -1;
    for (size_t k = 0; k + 1 < g_prime_cnt; ++k) {
        double ratio = (double)g_primes[k + 1] / (double)g_primes[k];
        if (ratio >= threshold) last_k = (int32_t)k;
    }
    return (seq_t)((last_k == -1) ? 1 : ((uint64_t)last_k + 2));
}
