/*
 * A072973  Denominator of b(n) = (50*n-6)/(binomial(3n,n)*2^n).
 * https://oeis.org/A072973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072973: denominator of (50*n-6)/(C(3*n,n)*2^n) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/pow_seq.h"   /* pow_seq(seq_t base, int exp) */

static uint64_t *primes = NULL;
static size_t primes_count = 0;
static idx_t primes_limit = 0;

/* Build a static list of primes up to limit (inclusive). */
static void ensure_primes(idx_t n)
{
    idx_t limit = 3 * n;
    if (limit <= primes_limit)
        return;                       /* already sufficient */

    /* free old list if any */
    free(primes);
    primes = NULL;
    primes_count = 0;

    size_t size_limit = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(size_limit);
    if (!is_prime)
        return;                     /* allocation failed: primes remain NULL */

    for (size_t i = 0; i < size_limit; ++i)
        is_prime[i] = true;

    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes and build array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        primes_count = 0;
        primes_limit = 0;
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = (uint64_t)i;
    free(is_prime);
    primes_count = cnt;
    primes_limit = limit;
}

/* Main function computing the OEIS sequence A072973. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    ensure_primes(n);

    /* Numerator of b(n) */
    uint64_t N = 50ULL * (uint64_t)n - 6ULL;

    seq_t denom = (seq_t)1;

    /* Factor the numerator and remove common factors from the denominator. */
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p > 3ULL * n)
            break;

        /* Exponent of p in the numerator (50*n-6). */
        uint64_t f = 0;
        uint64_t tmp = N;
        while (tmp % p == 0) {
            ++f;
            tmp /= p;
        }

        /* Exponent of p in C(3n,n) * 2^n. */
        uint64_t e = 0;
        uint64_t power = p;
        while (power <= 3 * n) {
            e += 3 * n / power - n / power - 2 * n / power;
            power *= p;
        }
        if (p == 2)
            e += n;      /* extra 2^n factor */

        /* Remaining exponent after cancellation. */
        if (e > f)
            e -= f;
        else
            e = 0;

        if (e > 0) {
            seq_t pw = pow_seq((seq_t)p, (int)e);
            denom = denom * pw;
        }
    }

    return denom;
}
