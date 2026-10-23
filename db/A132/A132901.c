/*
 * A132901  Numbers formed by concatenating 11 consecutive prime numbers.
 * https://oeis.org/A132901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(p_{n}, p_{n+1}, …, p_{n+10})
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 163,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k denotes the k-th prime number. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Cache of prime numbers. */
static uint64_t *prime_cache = NULL;
static size_t   prime_cnt   = 0;

/* Ensure that at least 'needed' primes are available in prime_cache.
   The function recomputes all primes up to a conservative upper bound
   whenever more are required. */
static void _ensure_primes(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    /* Upper bound for the n-th prime: n*(15) + 100 is far above the
       actual value for n ≤ 10000. */
    idx_t limit = needed * 15 + 100;

    /* Boolean sieve. */
    uint8_t *sieve = (uint8_t*)malloc((size_t)limit + 1);
    if (!sieve) exit(1);
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    /* Allocate new cache. */
    uint64_t *new_cache = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_cache) {
        free(sieve);
        exit(1);
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            new_cache[idx++] = i;

    free(sieve);
    free(prime_cache);
    prime_cache = new_cache;
    prime_cnt   = cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* We need the first n+10 primes for the n-th term. */
    _ensure_primes(n + 10);

    idx_t start = n - 1;          /* 0‑based index of the first prime */
    seq_t value = 0;

    for (idx_t i = 0; i < 11; ++i) {
        uint64_t p64 = prime_cache[start + i];
        seq_t   p   = (seq_t)p64;
        seq_t   mult = pow10((idx_t)digit_len(p));
        value = value * mult + p;
    }

    return value;
}
