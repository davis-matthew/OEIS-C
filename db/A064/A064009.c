/*
 * A064009  a(n) = Sum_{k=1..n} d(k)*prime(k), where d(k) = A001223.
 * https://oeis.org/A064009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ∑_{k=1..n} prime(k) * (prime(k+1) – prime(k))
 * Range:     u8 n <= 8, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;

/* Ensure prime_cache holds at least `need` primes (the first `need` primes). */
static void ensure_primes(size_t need)
{
    if (prime_cache != NULL && prime_cnt >= need)
        return;

    /* Free any previous cache. */
    if (prime_cache != NULL) {
        free(prime_cache);
        prime_cache = NULL;
        prime_cnt   = 0;
    }

    /* Acquire the first `need` primes. */
    uint32_t *buf = NULL;
    size_t   cnt  = 0;
    sieve_first_n_primes((idx_t)need, &buf, &cnt);    /* cnt == need */

    /* Copy into our cache of uint64_t to avoid other cast conflicts. */
    prime_cache = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    for (size_t i = 0; i < cnt; ++i)
        prime_cache[i] = (uint64_t)buf[i];

    free(buf);
    prime_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for indices below the offset */

    /* We need the first `n+1` primes for prime(k+1). */
    ensure_primes((size_t)n + 1);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pk  = (seq_t)prime_cache[(size_t)k - 1];
        seq_t pk1 = (seq_t)prime_cache[(size_t)k];
        sum += pk * (pk1 - pk);
    }
    return sum;
}
