/*
 * A101349  Numbers of cubes between prime(n) and prime(n+1).
 * https://oeis.org/A101349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the number of perfect cubes k^3 such that p_n < k^3 < p_{n+1},
 * where p_n is the n-th prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool initialized = false;
static idx_t prime_cnt = 0;
static uint64_t *primes = NULL;     /* primes[0] = 2   ...   primes[prime_cnt-1] */
static seq_t *a_vals = NULL;       /* a_vals[0] = a(1) ... a_vals[prime_cnt-2] = a(prime_cnt-1) */

/* Sieve of Eratosthenes up to LIMIT and fill primes array. */
static void init_primes(void)
{
    const uint64_t LIMIT = 200000ULL;          /* > 10001st prime (≈104743) */
    bool *is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();

    for (uint64_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    prime_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++prime_cnt;

    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) abort();

    idx_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
}

/* Precompute a_vals up to prime_cnt-1. */
static void init_a_vals(void)
{
    if (!prime_cnt) init_primes();

    /* All cubes up to LIMIT */
    seq_t cubes[1000];
    idx_t cube_cnt = 0;
    seq_t t = 1;
    while (1) {
        seq_t cube = t * t * t;
        if (cube > (seq_t)primes[prime_cnt-1]) break;
        cubes[cube_cnt++] = cube;
        ++t;
    }

    a_vals = (seq_t*)malloc((prime_cnt - 1) * sizeof(seq_t));
    if (!a_vals) abort();

    for (idx_t i = 1; i < prime_cnt; ++i) {
        uint64_t low  = primes[i-1];
        uint64_t high = primes[i];
        seq_t count = 0;

        for (idx_t j = 0; j < cube_cnt; ++j) {
            seq_t c = cubes[j];
            if (c <= low) continue;
            if (c >= high) break;
            ++count;
        }
        a_vals[i-1] = count;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!initialized) {
        init_a_vals();
        initialized = true;
    }

    if (n > prime_cnt - 1) return 0; /* undefined, return 0 */
    return a_vals[n - 1];
}
