/*
 * A233654  |{prime p < n: n - p = sigma(k) for some k > 0}|, where sigma(k)
 *          is the sum of all (positive) divisors of k.
 * https://oeis.org/A233654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1094, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A233654: a(n) = |{prime p < n : n - p = σ(k) for some k > 0}| */
/* Computation: pre‑compute divisor‑sums σ(k) up to the
   largest request and build a boolean table marking the values
   that appear.  Then generate all primes below the target
   and count those with n‑p in the σ‑table. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"

static seq_t   *sigma_arr    = NULL;
static bool    *sigma_present = NULL;
static uint64_t *prime_list   = NULL;
static size_t     prime_cnt   = 0;
static size_t     max_sieved  = 0;

/* Extend precomputation to cover all indices up to 'limit' (inclusive) */
static void
_ensure_sieve(uint64_t limit)
{
    if (limit <= max_sieved)          /* already covered */
        return;

    /* allocate fresh arrays of length limit+1 */
    size_t len = limit + 1;

    free(sigma_arr);
    sigma_arr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!sigma_arr) { /* errno handling omitted for brevity */ }

    free(sigma_present);
    sigma_present = (bool *)calloc(len, sizeof(bool));
    if (!sigma_present) { /* errno handling omitted for brevity */ }

    /* compute σ(k) for k = 1 .. limit */
    for (uint64_t i = 1; i <= limit; ++i) {
        for (uint64_t j = i; j <= limit; j += i)
            sigma_arr[j] += (seq_t)i;
    }

    /* mark values that occur as σ(k) */
    for (uint64_t k = 1; k <= limit; ++k) {
        seq_t v = sigma_arr[k];
        if ((uint64_t)v <= limit)
            sigma_present[(uint64_t)v] = true;
    }

    /* rebuild primes up to 'limit' */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    sieve_primes(limit, &prime_list, &prime_cnt);

    max_sieved = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, also a(2) handled below */
        return 0;

    uint64_t N = (uint64_t)n;

    /* we need primes < n and σ values up to n-1 */
    _ensure_sieve(N - 1);

    uint64_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p >= N)   /* primes are sorted */
            break;
        uint64_t m = N - p;
        if (m <= max_sieved && sigma_present[m])
            ++count;
    }
    return (seq_t)count;
}
