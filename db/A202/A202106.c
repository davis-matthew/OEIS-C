/*
 * A202106  Smallest prime p such that 4n^2+p is prime.
 * https://oeis.org/A202106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 4*n^2 + p is prime
 * Range:     u8 n <= 7, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
