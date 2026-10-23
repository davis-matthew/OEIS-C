/*
 * A024450  Sum of squares of the first n primes.
 * https://oeis.org/A024450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} prime(i)^2 Initial condition: a(1) = 2^2 = 4
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 637, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ========= static storage ========= */
static uint32_t *primes      = NULL;   /* primes[0] = 2 */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static seq_t   *sum_cache    = NULL;   /* a(i) values, 0‑based : sum_cache[0] = a(1) */
static size_t   cache_size   = 0;      /* number of a(i) values stored */

/* ---------- helpers ---------- */
/* Ensure that at least `n` primes are known.
   The caller guarantees that n ≥ 1. */
static void ensure_primes(idx_t n)
{
    size_t required = (size_t)n;
    if (primes_cnt >= required)
        return;                           /* already enough */

    /* Resize the primes buffer if needed */
    size_t capacity = primes ? primes_cnt * 2 : 16;
    while (capacity < required)
        capacity <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(primes, capacity * sizeof(uint32_t));
    if (!tmp) { free(primes); exit(1); }
    primes = tmp;

    /* Initialise the first prime if the table is empty */
    if (primes_cnt == 0) {
        primes[0] = 2;
        primes_cnt = 1;
    }

    /* Start testing from the next odd number after the largest known prime.
       Special case: if only 2 is known, start at 3. */
    uint32_t cand = (primes_cnt == 1) ? 3 : primes[primes_cnt - 1] + 2;

    while (primes_cnt < required) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > (uint64_t)cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
            primes[primes_cnt++] = cand;
        cand += 2;   /* skip even numbers */
    }
}

/* Ensure that the first `n` terms of the sequence are computed. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n;
    if (cache_size >= required)
        return;                           /* already computed */

    /* Resize the cache buffer if needed */
    size_t capacity = sum_cache ? cache_size * 2 : 16;
    while (capacity < required)
        capacity <<= 1;
    seq_t *tmp = (seq_t *)realloc(sum_cache, capacity * sizeof(seq_t));
    if (!tmp) { free(sum_cache); exit(1); }
    sum_cache = tmp;

    /* Compute missing cumulative sums */
    for (size_t i = cache_size; i < required; ++i) {
        seq_t sq = (seq_t)primes[i] * (seq_t)primes[i];
        if (i == 0)
            sum_cache[i] = sq;                     /* a(1) */
        else
            sum_cache[i] = sum_cache[i - 1] + sq;   /* a(i+1) = a(i) + prime(i+1)^2 */
    }
    cache_size = required;
}

/* ========= public function ========= */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for 0, harmless */
    ensure_primes(n);
    ensure_cache(n);
    return sum_cache[n - 1];                   /* array is 0‑based, n is 1‑based */
}
