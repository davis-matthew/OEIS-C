/*
 * A008793  The problem of the calissons: number of ways to tile a hexagon of
 *          edge n with diamonds of side 1. Also number of plane partitions
 *          whose Young diagrams fit inside an n X n X n box.
 * https://oeis.org/A008793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ∏_{i=0}^{n-1} i!·(i+2n)!/(i+n)!² (OEIS A008793)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  prime sieve ------------------------------------ */
static uint64_t *primes = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* create all primes ≤ limit, replace previous table if needed */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free old table */
    free(primes);
    primes = NULL;
    prime_count = 0;
    prime_limit = 0;

    /* simple Sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    uint8_t *sieve = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!sieve)
        abort();

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    /* count primes */
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++prime_count;

    /* allocate array */
    primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!primes)
        abort();

    /* store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            primes[pos++] = (uint64_t)i;

    free(sieve);
    prime_limit = limit;
}

/* ----------  Legendre exponent of prime p in m! --------------- */
static inline int legendre(idx_t m, uint64_t p)
{
    int e = 0;
    while (m) {
        m /= p;
        e += (int)m;
    }
    return e;
}

/* ----------  main sequence function --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t maxM = 3 * n;
    ensure_primes_upto(maxM);

    /* temporary exponent sums for each prime  */
    /* use long long to avoid overflow of intermediate sums   */
    long long *exp_sum = (long long *)calloc(prime_count, sizeof(long long));
    if (!exp_sum)
        abort();

    for (idx_t i = 0; i < n; ++i) {
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = primes[pi];
            int e = 0;
            e += legendre(i, p);
            e += legendre(i + 2 * n, p);
            e -= 2 * legendre(i + n, p);
            exp_sum[pi] += e;
        }
    }

    /* multiply all prime powers; intermediate result never exceeds the final result */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = primes[pi];
        long long e = exp_sum[pi];
        while (e-- > 0)
            result *= (seq_t)p;
    }

    free(exp_sum);
    return result;
}
