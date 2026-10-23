/*
 * A161911  a(n) is the n-th difference between consecutive primes minus the
 *          number of divisors of n.
 * https://oeis.org/A161911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A001223(n) - A000005(n)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime_gap(n) - divisor_count(n) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

static uint32_t *primes = NULL;          /* 0‑based list of primes   */
static size_t    prime_count = 0;         /* number of primes stored  */
static size_t    prime_limit  = 0;        /* sieve limit in the array */

static bool    ensure_primes(idx_t needed)
{
    if (prime_count >= (size_t)needed)
        return true;

    /* estimate a sieve limit larger than the needed prime index */
    size_t cnt = (size_t)needed;
    size_t limit;
    if (cnt < 6)
        limit = 15;
    else {
        double n = (double)cnt;
        double est = n * (log(n) + log(log(n)));
        limit = (size_t)(est) + 3;
    }
    if (limit <= prime_limit)
        limit = prime_limit * 2 + 100;
    if (limit < 11)
        limit = 11;

    /* allocate sieve array */
    uint8_t *is_prime = (uint8_t*)calloc(limit, 1);
    if (!is_prime)
        return false;

    for (size_t i = 2; i < limit; ++i)
        is_prime[i] = 1;

    size_t sq = (size_t)sqrt((double)limit) + 1;
    for (size_t i = 2; i <= sq; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect primes */
    size_t new_count = 0;
    for (size_t i = 2; i < limit; ++i)
        if (is_prime[i])
            ++new_count;

    uint32_t *new_primes = (uint32_t*)malloc(new_count * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return false;
    }

    size_t idx = 0;
    for (size_t i = 2; i < limit && idx < new_count; ++i) {
        if (is_prime[i]) {
            new_primes[idx++] = (uint32_t)i;
        }
    }

    free(is_prime);

    /* replace old arrays */
    free(primes);
    primes = new_primes;
    prime_count = new_count;
    prime_limit = limit;
    return true;
}

static seq_t divisor_count(idx_t n)
{
    seq_t result = 1;
    idx_t temp   = n;
    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        seq_t exp = 0;
        while (temp % primes[i] == 0) {
            temp /= primes[i];
            ++exp;
        }
        if (exp > 0)
            result *= (exp + 1);
    }
    if (temp > 1)
        result *= 2; /* remaining prime factor */
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* we need prime[n] (0‑based) and prime[n‑1] */
    if (!ensure_primes(n + 1))
        return 0; /* failed allocation */

    seq_t gap = (seq_t)primes[n] - (seq_t)primes[n - 1];
    seq_t divcnt = divisor_count(n);
    return gap - divcnt;
}
