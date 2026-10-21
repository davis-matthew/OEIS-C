/*
 * A068140  Smaller of two consecutive numbers each divisible by a cube
 *          greater than one.
 * https://oeis.org/A068140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 854, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the smaller of two consecutive integers, both divisible by a prime cube > 1.
   The function searches sequentially for such k and returns the nth one. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve up to a fixed limit – enough to cover the cube roots needed for the
   first several thousand terms. */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static int primes_inited = 0;
#define PRIME_LIMIT 200000   /* 200000^3 = 8·10^15, safely far beyond required bounds */

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = 1;

    /* Sieve of Eratosthenes */
    bool *is_comp = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_comp) abort();
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i) is_comp[i] = false;

    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) is_comp[j] = true;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) if (!is_comp[i]) ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) abort();
    prime_cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) if (!is_comp[i]) primes[prime_cnt++] = i;

    free(is_comp);
}

/* Determine whether a number has a prime cube divisor (greater than 1). */
static bool has_cube_factor(idx_t n)
{
    if (n < 8) return false; /* smallest cube >1 is 2^3 = 8 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t cub = p * p * p;
        if (cub > n) break;
        if (n % cub == 0) return true;
    }
    return false;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for offset 1; only called with n≥1. */
    init_primes();

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        if (has_cube_factor(k) && has_cube_factor(k + 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
