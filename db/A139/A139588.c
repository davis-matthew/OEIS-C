/*
 * A139588  Nonprime numbers with Fibonacci number of divisors.
 * https://oeis.org/A139588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Nonprime numbers with a Fibonacci number of divisors. a(n) is
 *            the nth integer, n ≥ 1, such that • n is not prime (1 is
 *            included), • d(n) (the number of positive divisors of n) is a
 *            Fibonacci number
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   make_fib_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all such numbers up to a fixed bound
   (2 000 000).  The bound easily yields more than ten thousand terms,
   which covers the test range.  The sieve uses a linear algorithm
   to obtain the smallest prime factors and divisor counts.  All
   computations respect the limits for seq_t and idx_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/make_fib_table.h"

#define SIEVE_LIMIT 2000000

static uint32_t *spf = NULL;          /* smallest prime factor */
static uint16_t *tau = NULL;          /* number of divisors */
static seq_t   *candidates = NULL;   /* list of sequence values */
static size_t   cand_len = 0;
static bool     initialized = false;

/* One‑time initialization routine */
static void init(void)
{
    idx_t N = SIEVE_LIMIT;

    /* Allocate arrays.  Use calloc so zeros are set (spf[1]==0). */
    spf  = (uint32_t*)calloc((size_t)N + 1, sizeof(uint32_t));
    tau  = (uint16_t*)calloc((size_t)N + 1, sizeof(uint16_t));
    if (!spf || !tau) abort();

    /* Linear sieve to fill spf[] and collect primes. */
    uint32_t *primes = (uint32_t*)malloc((N / 10) * sizeof(uint32_t));  /* about 150 000 primes */
    if (!primes) abort();
    size_t prime_cnt = 0;

    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t prod = i * (idx_t)p;
            if (prod > N) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }

    /* Compute divisor counts using the spf array. */
    tau[1] = 1;
    uint32_t maxTau = 1;
    for (idx_t i = 2; i <= N; ++i) {
        uint32_t p = spf[i];
        idx_t    m = i / p;
        uint32_t exp = 1;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        tau[i] = tau[m] * (exp + 1);
        if (tau[i] > maxTau) maxTau = tau[i];
    }

    /* Build table of Fibonacci numbers up to maxTau. */
    bool *isFib = make_fib_table(maxTau);

    /* Allocate candidates array (estimate size as N/8). */
    size_t est = N / 8;
    candidates = (seq_t*)malloc(est * sizeof(seq_t));
    if (!candidates) abort();

    /* Gather sequence elements. */
    for (idx_t i = 1; i <= N; ++i) {
        bool isComposite;
        if (i == 1)
            isComposite = true;                     /* 1 is considered non‑prime */
        else
            isComposite = (spf[i] != (uint32_t)i);   /* composite if spf ≠ itself */

        if (isComposite && isFib[tau[i]]) {
            if (cand_len == est) {   /* grow array if needed */
                est *= 2;
                candidates = (seq_t*)realloc(candidates, est * sizeof(seq_t));
                if (!candidates) abort();
            }
            candidates[cand_len++] = (seq_t)i;
        }
    }

    free(isFib);
    free(primes);
    initialized = true;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, defined as 0 here */
    if (!initialized) init();
    if ((idx_t)n > cand_len) abort();   /* safety: out of bounds */
    return candidates[n - 1];
}
