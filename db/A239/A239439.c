/*
 * A239439  Smallest number between consecutive odd primes whose sum of prime
 *          factors is a minimum.
 * https://oeis.org/A239439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest integer between consecutive odd primes whose sum
 *            (with multiplicity) of prime factors is minimal
 * Range:     u8 n <= 52, u16 n <= 6540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- local tables ---------- */
static uint32_t *primes     = NULL;   /* all primes, primes[0] = 2 */
static size_t   primes_sz  = 0;      /* number of primes stored */

static uint32_t *spf        = NULL;   /* smallest prime factor, 0..limit */
static uint32_t *sum_pf     = NULL;   /* sum of prime factors (with multiplicity) */
static size_t   spf_limit   = 0;      /* current limit of spf and sum_pf */

/* ---------- helper ---------- */
/* integer square root for unsigned 64‑bit values */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* rebuild tables up to at least 'limit' */
static void recompute(idx_t limit)
{
    /* free old memory */
    if (primes)    free(primes);
    if (spf)       free(spf);
    if (sum_pf)    free(sum_pf);

    /* allocate spf and sum_pf */
    spf    = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    sum_pf = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf || !sum_pf) exit(1);

    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) spf[i] = (uint32_t)i;

    idx_t sq = int_sqrt(limit);
    for (idx_t i = 2; i <= sq; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
        }
    }

    /* collect primes and compute sum_pf */
    size_t cap = limit / 10 + 10;
    primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    primes_sz = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {
            if (primes_sz >= cap) {
                cap *= 2;
                primes = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[primes_sz++] = (uint32_t)i;
        }
    }

    sum_pf[0] = 0;
    if (limit >= 1) sum_pf[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        sum_pf[i] = sum_pf[i / spf[i]] + spf[i];

    spf_limit = limit;
}

/* ensure that we have enough primes to evaluate a(n) */
static void ensure_precomputed(idx_t n)
{
    /* need primes[0..n+1] inclusive (primes[0]==2) */
    while (primes_sz <= (size_t)n + 1) {
        idx_t new_limit = spf_limit ? spf_limit * 2 : 1000;
        if (new_limit < (idx_t)(n + 2) * 20) new_limit = (idx_t)(n + 2) * 20;
        recompute(new_limit);
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_precomputed(n);

    uint32_t p = primes[(size_t)n];          /* nth odd prime */
    uint32_t q = primes[(size_t)n + 1];      /* (n+1)st odd prime */

    uint32_t best_val = p + 1;
    uint32_t best_sum = sum_pf[best_val];

    for (uint32_t val = best_val + 1; val < q; ++val) {
        uint32_t s = sum_pf[val];
        if (s < best_sum || (s == best_sum && val < best_val)) {
            best_sum = s;
            best_val = val;
        }
    }
    return (seq_t)best_val;
}
