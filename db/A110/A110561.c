/*
 * A110561  Denominators of T(n+1)/n! reduced to lowest terms, where T(n) are
 *          the triangular numbers A000217.
 * https://oeis.org/A110561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of ((n+1)(n+2))/(2·n!) reduced to lowest
 *            terms The denominator equals (2·n!)/gcd((n+1)(n+2),2·n!). This
 *            implementation avoids computing the large factorial explicitly
 *            by using prime factorisation. Runtime is O(n/log n) per call,
 *            with a small constant
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow_seq_p, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"
#include "../utils/pow_seq_p.h"

static uint32_t *prime_list = NULL;  /* array of primes */
static size_t prime_count = 0;       /* number of primes */
static uint32_t prime_limit = 0;     /* largest n for which primes have been generated */

seq_t a(idx_t n)
{
    /* ensure prime list covers at least n */
    if (!prime_list || n > prime_limit) {
        if (prime_list) free(prime_list);
        simple_sieve((uint32_t)n, &prime_list, &prime_count);
        prime_limit = (uint32_t)n;
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p > n) break;            /* primes greater than n give zero denominator exponent */

        /* exponent of p in n! */
        idx_t exp_fact = 0;
        idx_t div = p;
        while (div <= n) {
            exp_fact += n / div;
            if (div > n / p) break;
            div *= p;
        }

        /* exponent of p in the denominator (2·n!) */
        idx_t exp_den = exp_fact + (p == 2 ? 1 : 0);

        /* exponent of p in the numerator (n+1)(n+2) */
        idx_t exp_num = 0;
        idx_t temp = n + 1;
        while (temp % p == 0) { ++exp_num; temp /= p; }
        temp = n + 2;
        while (temp % p == 0) { ++exp_num; temp /= p; }

        /* exponent remaining in denominator after reducing the fraction */
        idx_t diff = (exp_den > exp_num) ? (exp_den - exp_num) : 0;
        if (diff > 0) {
            seq_t p_pow = pow_seq_p(p, diff);
            result *= p_pow;
        }
    }

    return result;
}
