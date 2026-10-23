/*
 * A175508  a(n) is the difference of n and the product of the indices of
 *          prime factors of n with multiplicity, a(1) = 0.
 * https://oeis.org/A175508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (product of indices of prime factors of n with
 *            multiplicity), with OEIS convention a(1) = 1
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *primes = NULL;
static size_t prime_count = 0;
static uint64_t sieve_limit = 0;

static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;
    if (primes) {
        free(primes);
        primes = NULL;
    }
    build_primes_upto(limit, &primes, &prime_count);
    sieve_limit = limit;
}

static idx_t get_prime_index(uint64_t p)
{
    size_t lo = 0;
    size_t hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] == p)
            return (idx_t)(mid + 1);    /* 1‑based index */
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;    /* should never happen for valid prime p */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    ensure_sieve((uint64_t)n);

    /* factorise n */
    uint64_t fpr[64], fex[64];
    size_t fcnt;
    factor_u64((uint64_t)n, fpr, fex, &fcnt);

    seq_t prod = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        idx_t pidx = get_prime_index(fpr[i]);
        for (unsigned j = 0; j < fex[i]; ++j)
            prod *= (seq_t)pidx;
    }

    return (seq_t)n - prod;
}
