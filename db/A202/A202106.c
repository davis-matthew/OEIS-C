/*
 * A202106  Smallest prime p such that 4n^2+p is prime.
 * https://oeis.org/A202106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 4*n^2 + p is prime
 * Range:     u8 n <= 7, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    const uint64_t LIMIT = 10000000ULL;          /* 10 million – more than enough for n ≤ 10 000 */
    unsigned char *is_comp = (unsigned char *)calloc((size_t)(LIMIT + 1), 1);
    if (!is_comp) { primes_initialized = true; return; }

    /* Rough estimate of the number of primes below LIMIT: about LIMIT / log(LIMIT) */
    size_t est = (size_t)(LIMIT / 10);          /* generous over‑estimate to keep allocation simple */
    prime_list = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!prime_list) { free(is_comp); primes_initialized = true; return; }

    size_t pc = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (pc < est) prime_list[pc++] = i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    prime_cnt = pc;
    free(is_comp);
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (!primes_initialized) init_primes();
    if (prime_cnt == 0) return 0;                       /* should not happen */

    seq_t ns      = (seq_t)n;
    seq_t base    = 4ULL * ns * ns;                     /* 4*n² – fits in seq_t comfortably for n ≤ 10 000 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p       = (seq_t)prime_list[i];
        seq_t cand    = base + p;
        if (is_prime_big(cand)) return p;
    }

    /* If all pre‑computed primes were exhausted, continue with larger primes. */
    uint64_t p = (uint64_t)next_prime(prime_list[prime_cnt - 1] + 1);
    while (true) {
        seq_t cand = base + (seq_t)p;
        if (is_prime_big(cand)) return (seq_t)p;
        p = (uint64_t)next_prime(p + 1);
    }
}
