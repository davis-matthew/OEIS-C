/*
 * A143582  Denominators of coefficient of x^(n+1/2) in the series expansion
 *          of the haversine.
 * https://oeis.org/A143582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the reduced fraction 2 * (2k)! / ( (k!)^2
 *            * 4^k * (2k+1) ), with k = n-1 (equivalently, coefficient of
 *            z^(n+1/2) in 2*arcsin(sqrt(z)))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Exponent of prime p in m! (Legendre's formula). */
static idx_t legendre(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* Exponent of prime p in integer m. */
static idx_t exp_in_int(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m % p == 0) {
        ++e;
        m /= p;
    }
    return e;
}

/* Fast exponentiation for seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Prime sieve up to current maximum limit. */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t max_prime_limit = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= max_prime_limit)
        return;

    /* Allocate array to mark primes. */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return; /* allocation failure: abort silently */

    memset(is_prime, true, sz);
    if (sz > 0)
        is_prime[0] = false;
    if (sz > 1)
        is_prime[1] = false;

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and build list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    free(prime_list);
    prime_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            prime_list[idx++] = i;
    }

    free(is_prime);
    prime_count = cnt;
    max_prime_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but should not be used */

    idx_t k = n - 1;                /* coefficient index */
    if (k == 0)
        return 1;                   /* first term has denominator 1 */

    /* Prepare primes up to 2k+1. */
    ensure_primes((k << 1) + 1);

    seq_t denom = 1;

    const idx_t two_k = k << 1;
    const idx_t two_k_plus_one = two_k + 1;

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = prime_list[pi];
        if (p > two_k_plus_one)
            break;

        /* Exponents in numerator and denominator. */
        idx_t exp_num = ((p == 2) ? 1 : 0)
                      + legendre(two_k, p)
                      - 2 * legendre(k, p);

        idx_t exp_den = ((p == 2) ? two_k : 0)
                      + exp_in_int(two_k_plus_one, p);

        if (exp_den > exp_num) {
            idx_t diff = exp_den - exp_num;
            denom *= pow_seq((seq_t)p, diff);
        }
    }

    return denom;
}
