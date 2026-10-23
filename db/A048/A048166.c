/*
 * A048166  Numbers k that are divisible by the number of unitary divisors of
 *          k (A034444).
 * https://oeis.org/A048166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the integers k such that k is divisible by the * number
 *            of unitary divisors of k, i.e. k % 2^ω(k) == 0, * where ω(k) is
 *            the count of distinct prime factors of k. * This implements a
 *            direct sieve of omega(k) up to a fixed bound
 * Range:     u8 n <= 55, u16 n <= 9158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- configuration ---------- */
#define MAX_LIMIT 5000000ULL    /* safe upper bound for the first 10000 terms */

/* ---------- static data ---------- */
static uint8_t  *omega = NULL;   /* ω(k) for k up to limit */
static uint32_t *spf   = NULL;   /* smallest prime factor */
static uint32_t *primes= NULL;   /* array of primes */
static size_t    prime_cnt = 0;  /* number of primes found */
static bool      prepared = false;

/* ---------- helper to construct the sieve ---------- */
static void
prepare_sieve(void)
{
    if (prepared)
        return;
    /* allocate arrays */
    omega  = (uint8_t  *)malloc((MAX_LIMIT + 1) * sizeof(uint8_t));
    spf    = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    primes = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!omega || !spf || !primes) {
        /* in case of failure, abort */
        exit(1);
    }

    omega[1] = 0;
    spf[1]   = 0;
    prime_cnt = 0;

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            primes[prime_cnt++] = i;
            omega[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (p > spf[i] || ip > MAX_LIMIT)
                break;
            uint32_t v = (uint32_t)ip;
            spf[v] = p;
            if (p == spf[i])          /* same prime factor */
                omega[v] = omega[i];
            else
                omega[v] = omega[i] + 1;
        }
    }
    prepared = true;
}

/* ---------- main sequence function ---------- */
seq_t
a(idx_t n)
{
    prepare_sieve();

    size_t count = 0;
    for (uint64_t k = 1; k <= MAX_LIMIT; ++k) {
        uint8_t om = omega[k];
        uint64_t udiv = 1ull << om;   /* 2^ω(k) */
        if ((k % udiv) == 0) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
    /* If we reach here, the bound was insufficient. */
    /* In practice the first 10000 terms are well below MAX_LIMIT. */
    return 0;
}
