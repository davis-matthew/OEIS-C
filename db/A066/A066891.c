/*
 * A066891  Max (k : prime(k+n)-prime(k) = 0 mod(k)).
 * https://oeis.org/A066891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max { k | prime(k+n)-prime(k) ≡ 0 (mod k) } (offset 1)
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   issqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/issqrt.h"

/* Prime sieve up to a fixed limit sufficient for all n ≤ 10000. */
#define PRIME_LIMIT 5000000          /* 5·10^6 gives about 3.5·10^5 primes */
#define PRIME_CAPACITY 350000        /* capacity for the prime array */

static idx_t *primes = NULL;      /* primes[i] = (i+1)-th prime, 0‑based */
static size_t primes_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    /* Simple Sieve of Eratosthenes. */
    char *sieve = (char *)malloc(PRIME_LIMIT + 1);
    if (!sieve) exit(1);

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    size_t sq = issqrt(PRIME_LIMIT);
    for (size_t i = 2; i <= sq; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    primes = (idx_t *)malloc(PRIME_CAPACITY * sizeof(idx_t));
    if (!primes) { free(sieve); exit(1); }

    for (size_t i = 2; i <= PRIME_LIMIT && primes_count < PRIME_CAPACITY; ++i)
        if (sieve[i]) primes[primes_count++] = (idx_t)i;

    free(sieve);
    primes_initialized = true;
}

/* Return a(n) for n ≥ 1.  The function works for any order of calls. */
seq_t a(idx_t n)
{
    if (!primes_initialized) init_primes();

    /* Empirical bound: k is usually < 8·n, use 20·n for safety. */
    idx_t bound = 20 * n + 100;

    /* Ensure we do not request primes beyond those computed. */
    if (bound + n > (idx_t)primes_count)
        bound = (primes_count > n) ? (primes_count - n) : 1;
    if (bound < 1) bound = 1;

    seq_t max_k = 0;
    for (idx_t k = 1; k <= bound; ++k) {
        idx_t idx_hi = k + n - 1;   /* zero‑based indices */
        idx_t idx_lo = k - 1;
        seq_t diff = (seq_t)(primes[idx_hi] - primes[idx_lo]);

        if (diff % k == 0)
            max_k = (seq_t)k;
    }
    return max_k;
}
