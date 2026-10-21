/*
 * A061214  Product of composite numbers between the n-th and (n+1)st primes.
 * https://oeis.org/A061214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of composite numbers between the n-th and
 *            (n+1)-st primes
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 29, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static uint32_t *g_primes = NULL;
static size_t     g_prime_cnt = 0;
static bool      *g_is_prime = NULL;
static size_t     g_limit    = 0;

/* Ensure that at least n_needed primes have been generated. */
static void ensure_primes(idx_t n_needed)
{
    if (g_prime_cnt >= (size_t)n_needed) return;

    /* Estimate upper bound for the n-th prime */
    uint64_t n = (uint64_t)n_needed;
    size_t est;
    if (n < 6) {
        est = 15;
    } else {
        double d = n * (log((double)n) + log(log((double)n)));
        est = (size_t)(d + 3.0);
    }
    if (est < g_limit) est = g_limit;   /* keep at least the previous limit */

    /* Allocate new sieve array */
    bool *new_is_prime = (bool *)malloc((est + 1) * sizeof(bool));
    if (!new_is_prime) exit(1);
    for (size_t i = 0; i <= est; ++i) new_is_prime[i] = true;
    new_is_prime[0] = new_is_prime[1] = false;
    for (size_t i = 2; i * i <= est; ++i) {
        if (new_is_prime[i]) {
            for (size_t j = i * i; j <= est; j += i)
                new_is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= est; ++i)
        if (new_is_prime[i]) ++cnt;

    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= est; ++i)
        if (new_is_prime[i]) new_primes[idx++] = (uint32_t)i;

    /* Replace old data */
    free(g_is_prime);
    free(g_primes);
    g_is_prime = new_is_prime;
    g_primes   = new_primes;
    g_prime_cnt = cnt;
    g_limit     = est;
}

/* OEIS sequence A061214 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1, return 0 */

    ensure_primes(n + 1);                 /* need (n+1)-th prime */
    uint32_t p_n    = g_primes[n - 1];
    uint32_t p_next = g_primes[n];

    seq_t prod = (seq_t)1;
    for (idx_t x = (idx_t)p_n + 1; x < (idx_t)p_next; ++x) {
        if (!g_is_prime[(size_t)x])      /* composite numbers only */
            prod *= (seq_t)x;
    }
    return prod;
}
