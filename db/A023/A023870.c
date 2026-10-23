/*
 * A023870  a(n) = 1*prime(n) + 2*prime(n-1) + ... + k*prime(n+1-k), where
 *          k=floor((n+1)/2) and prime(n) is the n-th prime.
 * https://oeis.org/A023870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over i=1..floor((n+1)/2) of i*prime(n+1-i)
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;
static idx_t primes_count = 0;

static void ensure_primes(idx_t n)
{
    if (n <= primes_count)
        return;
    /* allocate primes up to n */
    size_t new_count = (size_t)n;
    uint32_t *tmp = NULL;
    size_t cnt = 0;
    sieve_first_n_primes((idx_t)new_count, &tmp, &cnt);
    if (cnt != new_count) {
        /* In a real implementation we might handle errors, but
           the build system guarantees correct limits. */
        return;
    }
    free(primes);
    primes = tmp;
    primes_count = (idx_t)new_count;
}

seq_t a(idx_t n)
{
    /* Worst-case k ≈ (n+1)/2, so we need primes up to index n. */
    if (n == 0)
        return 0;
    ensure_primes(n);
    idx_t k = (n + 1) / 2;
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t idx = n + 1 - i;        /* prime index, 1-based */
        seq_t prime_val = (seq_t)primes[idx - 1];
        sum += (seq_t)i * prime_val;
    }
    return sum;
}
