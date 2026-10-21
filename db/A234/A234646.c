/*
 * A234646  Sum of the distinct prime divisors of n^3 + 1.
 * https://oeis.org/A234646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct prime divisors of n^3 + 1
 * Range:     u8 n <= 15, u16 n <= 266, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime sieve up to 1 000 000 (enough for n ≤ 10 000, since
   (10 000)^3 ≈ 10^12 and sqrt(10^12) ≈ 10^6). */
static const size_t PRIME_LIMIT = 1000000U;
static size_t primes_cnt = 0;
static uint64_t *primes = NULL;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;

    /* Allocate array for the sieve (true = composite) */
    bool *is_comp = (bool *)malloc(PRIME_LIMIT + 1);
    if (!is_comp)
        exit(EXIT_FAILURE);

    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_comp[i] = false;

    /* Eratosthenes sieve */
    for (size_t i = 2; (size_t)i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            ++count;

    primes_cnt = count;
    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes)
        exit(EXIT_FAILURE);

    /* Store primes */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[idx++] = i;
        }
    }

    free(is_comp);
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute n^3 + 1 using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 val128 = tn * tn * tn + 1;
    uint64_t val = (uint64_t)val128; /* fits because n ≤ 10 000 */

    init_primes();

    uint64_t temp = val;
    seq_t sum = 0;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            sum += (seq_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)            /* remaining prime factor > sqrt(original) */
        sum += (seq_t)temp;

    return sum;
}
