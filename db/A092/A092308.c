/*
 * A092308  For p=prime(n), a(n) = the number of primes q such that q-1
 *          divides p-1.
 * https://oeis.org/A092308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A092308:  a(n) = the number of primes q such that
 *  q - 1 divides prime(n) - 1.
 *
 *  We compute prime(n) with a dynamic sieve and then enumerate all
 *  divisors of p-1.  For each divisor d we test whether d+1 is prime
 *  by using the sieve.  The result fits comfortably in seq_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------  Sieve infrastructure  ------------------- */
static uint8_t *is_composite = NULL;   /* 0 → prime, 1 → composite   */
static size_t  sieve_limit   = 0;       /* last index that was sieved  */

static uint32_t *prime_list = NULL;    /* array of primes            */
static size_t   prime_cnt   = 0;        /* number of primes stored    */

/* Build or rebuild a sieve up to 'limit' (inclusive). */
static void rebuild_sieve(size_t limit) {
    free(is_composite);
    is_composite = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_composite) exit(EXIT_FAILURE);

    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1U;

    free(prime_list);
    /* Allocate space for ~limit / ln(limit) primes; a small overhead */
    size_t alloc_size = limit / 10 + 10;
    prime_list = (uint32_t *)malloc(alloc_size * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            prime_list[prime_cnt++] = (uint32_t)i;

    sieve_limit = limit;
}

/* Ensure the sieve covers at least 'min_limit' values.            */
static void ensure_sieve(size_t min_limit) {
    if (sieve_limit >= min_limit) return;
    rebuild_sieve(min_limit);
}

/* ----------------  Factorisation helper  --------------------- */
#include "../utils/_factor_u64.h"         /* _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt) */

/* ------------------------------------------ */
/*  Recursive divisor enumeration and count  */
static void count_divisors(const uint64_t *pr,
                           const uint64_t *exp,
                           size_t    idx,
                           uint64_t  cur,
                           uint64_t  p,          /* upper bound for q = d+1            */
                           seq_t   *cnt) {
    if (idx == 0) {
        uint64_t q = cur + 1;            /* candidate prime */
        if (q <= p && !is_composite[q]) (*cnt)++;
        return;
    }
    uint64_t prime = pr[idx - 1];
    uint64_t e     = exp[idx - 1];
    uint64_t val   = 1;

    for (uint64_t i = 0; i <= e; ++i) {
        count_divisors(pr, exp, idx - 1, cur * val, p, cnt);
        val *= prime;
    }
}

/* ---------------------  Main sequence function  --------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined; safe fallback      */

    /* Initial sieve size: enough for the first ~17984 primes (≤ 200 000) */
    static int initialized = 0;
    if (!initialized) {
        rebuild_sieve(200000);
        initialized = 1;
    }

    /* Make sure we have at least n primes available.  Double the limit
     * until we reach that goal. */
    while (prime_cnt < (size_t)n) {
        size_t new_limit = sieve_limit * 2;
        ensure_sieve(new_limit);
    }

    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = prime_list[n - 1];
    uint64_t m = (uint64_t)p - 1;          /* divisor target */

    /* Factor m using the generic factorisation routine. */
    uint64_t factors[64];
    uint64_t expos[64];
    size_t  nf = 0;
    _factor_u64(m, factors, expos, &nf);

    /* Count divisors d of m with d+1 prime.  q = d+1 must also be ≤ p because
     * d ≤ m = p-1.  We use the sieve for primality testing. */
    seq_t result = 0;
    count_divisors(factors, expos, nf, 1, p, &result);

    return result;
}
