/*
 * A074167  Product of prime divisors of composite numbers between consecutive
 *          primes.
 * https://oeis.org/A074167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 29, u128 n <= 98,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = product of distinct prime factors of all composite
 * numbers strictly between the n-th prime and the (n+1)-th prime. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static uint64_t *primes = NULL;   /* primes[0] == 2 */
static size_t prime_cnt = 0;

/* Ensure that primes[] contains at least 'need' primes (1‑based). */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;
    idx_t limit = _prime_limit_for_idx(need);
    uint64_t *new_primes;
    size_t new_cnt;
    build_primes_upto(limit, &new_primes, &new_cnt);
    free(primes);
    primes = new_primes;
    prime_cnt = new_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is undefined; return 0 for safety */
        return 0;
    /* we need primes up to index n+1 (1‑based) */
    ensure_primes(n + 1);

    seq_t result = 1;

    uint64_t p1 = primes[n - 1];   /* n-th prime (1‑based) */
    uint64_t p2 = primes[n];       /* (n+1)-st prime */

    /* buffers for factorization */
    uint64_t pf[64], exps[64];
    size_t cnt;

    for (uint64_t m = p1 + 1; m < p2; ++m) {
        if (is_prime(m))
            continue;              /* skip prime numbers */
        factor_u64(m, pf, exps, &cnt);
        seq_t rad = 1;
        for (size_t i = 0; i < cnt; ++i)
            rad *= (seq_t)pf[i];
        result *= rad;
    }

    return result;
}
