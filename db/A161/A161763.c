/*
 * A161763  Product of the two primes with indices equal to the members of the
 *          n-th twin prime pair.
 * https://oeis.org/A161763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(A001359(n))*A000040(A006519(n)) Algorithm:
 *            precompute all primes up to a fixed bound, collect all
 *            twin‑prime pairs, and answer queries by indexing into the prime
 *            table. The bound (20 000 000) guarantees at least 90 000 twin
 *            pairs, enough for n≈10 000
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool initialized = false;
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static uint32_t *twin_lower = NULL;
static uint32_t *twin_upper = NULL;
static size_t twin_count = 0;

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Bound chosen to give >90 000 twin pairs (≈20 M primes). */
    const idx_t SIEVE_LIMIT = 20000000ULL;
    bool *is_prime = (bool *)malloc((size_t)SIEVE_LIMIT + 1);
    if (!is_prime) return;  /* allocation failed */

    for (size_t i = 0; i <= (size_t)SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= (size_t)SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes to allocate exact size. */
    for (size_t i = 2; i <= (size_t)SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++primes_count;

    primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    if (!primes) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)SIEVE_LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);

    /* Estimate max twin pairs < primes_count; allocate same size. */
    twin_lower = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    twin_upper = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    if (!twin_lower || !twin_upper) return;

    /* Find twin primes. */
    for (size_t i = 1; i < primes_count; ++i)
        if (primes[i] - primes[i-1] == 2) {
            twin_lower[twin_count] = primes[i-1];
            twin_upper[twin_count] = primes[i];
            ++twin_count;
        }
}

seq_t a(idx_t n)
{
    if (!initialized) init();
    if ((idx_t)(n-1) >= twin_count) return 0;     /* beyond pre‑computed range */

    uint32_t lower  = twin_lower[n-1];
    uint32_t upper  = twin_upper[n-1];
    seq_t v = (seq_t)primes[lower-1] * (seq_t)primes[upper-1];
    return v;
}
