/*
 * A087712  a(1) = 1; if n = k-th prime, a(n) = k; otherwise write all prime
 *          factors of n in nondecreasing order, replace each prime with its
 *          rank, and concatenate the ranks.
 * https://oeis.org/A087712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, digits, factor_u64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A087712: a(1)=1. For n>1, if n is the k‑th prime a(n)=k; otherwise
   a(n) is the concatenation (in decimal) of the ranks of the prime factors
   of n, written in non‑decreasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

static size_t prime_index(uint64_t p) {
    size_t lo = 0, hi = prime_cache_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_cache[mid] == p) return mid + 1;          /* rank = index+1 */
        if (prime_cache[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0; /* not found */
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* Ensure we have all primes up to n cached */
    if (n > prime_cache_limit) {
        if (prime_cache) free(prime_cache);
        build_primes_upto(n, &prime_cache, &prime_cache_cnt);
        prime_cache_limit = n;
    }

    /* Check if n is prime */
    size_t rank = prime_index((uint64_t)n);
    if (rank != 0) return (seq_t)rank;      /* n is prime */

    /* n is composite: factor and concatenate ranks */
    uint64_t facs[32];
    uint64_t exps[32];
    size_t fac_cnt;
    factor_u64((uint64_t)n, facs, exps, &fac_cnt);

    seq_t result = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        size_t r = prime_index(facs[i]);          /* rank of this prime factor */
        int d = digits((idx_t)r);                 /* number of decimal digits of the rank */
        seq_t pow = pow10((idx_t)d);              /* 10^d as seq_t */

        for (uint64_t rep = 0; rep < exps[i]; ++rep) {
            result = result * pow + (seq_t)r;     /* append rank as digits */
        }
    }
    return result;
}
