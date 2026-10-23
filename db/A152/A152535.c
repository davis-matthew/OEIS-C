/*
 * A152535  a(n) = n*prime(n) - Sum_{i=1..n} prime(i).
 * https://oeis.org/A152535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*prime(n) - Sum_{i=1..n} prime(i)
 * Range:     u8 n <= 12, u16 n <= 145, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for primes and cumulative sums */
static bool    primes_built = false;
static uint32_t *primes    = NULL;
static uint64_t *cumprimes = NULL;
static size_t   prime_count = 0;

/* Build the first `LIMIT` primes and their cumulative sums.
   LIMIT = 200000 is enough to contain at least the first 10000 primes. */
static void build_primes(void)
{
    if (primes_built)
        return;

    const size_t LIMIT = 200000;          /* ≈ 104 729 is the 10 000‑th prime */
    bool *sieve = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve)
        return;                            /* allocation failed – leave empty */

    for (size_t i = 0; i <= LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    cumprimes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes || !cumprimes) {
        free(sieve);
        free(primes);
        free(cumprimes);
        return;
    }

    size_t idx = 0;
    uint64_t acc = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            primes[idx]    = (uint32_t)i;
            acc += (uint64_t)i;
            cumprimes[idx] = acc;
            ++idx;
        }
    }

    prime_count = cnt;
    primes_built = true;

    free(sieve);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;            /* offset 1: a(1) is defined, below is ignored */

    if (!primes_built)
        build_primes();

    if ((size_t)n > prime_count)    /* out of range for the chosen limit */
        return (seq_t)0;

    uint64_t term = (uint64_t)n * (uint64_t)primes[n - 1] - cumprimes[n - 1];
    return (seq_t)term;             /* cast to seq_t, allowing natural truncation */
}
