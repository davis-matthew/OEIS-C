/*
 * A133227  Incorrect duplicate of A133221.
 * https://oeis.org/A133227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = product of the first ⌈n/2⌉ odd primes, with a(1)=1
 *            a(1)=1; for n≥2, let k=(n+1)/2 (the ceiling of n/2),
 *            a(n)=∏_{i=1}^{k} p_{i+1}, where p_i is the i‑th prime (p_1=2)
 * Range:     i8 n <= 6, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Limit for pre‑computing primes – sufficient for n≈10000 */
#define PRIME_LIMIT 500000
/* Max number of primes we keep. The count of primes ≤ PRIME_LIMIT is 41537 */
#define MAX_PRIMES 50000

static uint32_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    size_t limit = PRIME_LIMIT;
    char *sieve = (char *)calloc(limit + 1, 1);
    if (!sieve)
        return; /* allocation failed – nothing we can do */

    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    size_t idx = 0;
    for (uint32_t i = 2; i <= limit && idx < MAX_PRIMES; ++i) {
        if (!sieve[i])
            primes[idx++] = i;
    }
    primes_cnt = idx;
    free(sieve);
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    idx_t k = (n + 1) / 2;        /* ceil(n/2) */
    init_primes();

    /* Ensure we have enough primes (should hold for tested ranges) */
    if (k + 1 > primes_cnt)        /* we need primes[1..k] i.e. k+1 primes */
        return (seq_t)0;           /* fallback – should not happen */

    seq_t prod = (seq_t)1;
    for (size_t i = 1; i <= (size_t)k; ++i)  /* skip p_1 = 2 */
        prod *= (seq_t)primes[i];

    return prod;
}
