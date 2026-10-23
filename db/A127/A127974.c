/*
 * A127974  Numerators in expansion of (1-x)^(-2/3).
 * https://oeis.org/A127974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator(n! / Product_{k=0..n-1} (2+3*k))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

/* Build sieve of primes up to 'up_to' and return array and count. */
static void build_primes(idx_t up_to, uint32_t **primes_ptr, size_t *cnt_ptr)
{
    if (up_to < 2) {
        *primes_ptr = NULL;
        *cnt_ptr   = 0;
        return;
    }
    size_t size = (size_t)up_to + 1;
    char *is_prime = (char *)malloc(size);
    if (!is_prime) exit(1);
    for (size_t i = 0; i < size; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= up_to; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= (uint32_t)up_to; j += i)
                is_prime[j] = 0;
        }
    }
    /* Count primes. */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)up_to; ++i)
        if (is_prime[i]) ++cnt;
    uint32_t *pr = NULL;
    if (cnt) {
        pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
        if (!pr) exit(1);
        size_t idx = 0;
        for (size_t i = 2; i <= (size_t)up_to; ++i)
            if (is_prime[i]) pr[idx++] = (uint32_t)i;
    }
    free(is_prime);
    *primes_ptr = pr;
    *cnt_ptr    = cnt;
}

/* Binary search for prime 'p' in 'primes' array. Returns count if not found. */
static size_t find_prime_index(uint32_t p, const uint32_t *primes, size_t cnt)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return cnt; /* not found */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Build primes up to 3*n. */
    idx_t limit = n * 3;
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(limit, &primes, &prime_cnt);

    /* Remaining exponent of each prime from n! to cancel with the product. */
    idx_t *rem = (prime_cnt > 0) ? (idx_t *)calloc(prime_cnt, sizeof(idx_t)) : NULL;

    /* Compute exponents of primes in n! */
    for (size_t j = 0; j < prime_cnt; ++j) {
        uint32_t p = primes[j];
        if (p > (uint32_t)n) break;
        idx_t m = p;
        while (m <= n) {
            rem[j] += n / m;
            m *= p;
        }
    }

    seq_t result = (seq_t)1;

    /* Process each term (2 + 3*k) for k = 0 .. n-1. */
    for (idx_t k = 0; k < n; ++k) {
        uint32_t term = 3 * (uint32_t)k + 2;
        uint32_t tmp   = term;

        for (size_t j = 0; j < prime_cnt && primes[j] * primes[j] <= tmp; ++j) {
            uint32_t p = primes[j];
            if (p > tmp) break;
            uint32_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (cnt) {
                idx_t used = (cnt <= rem[j]) ? cnt : rem[j];
                rem[j] -= used;
                uint32_t left = cnt - used;
                for (uint32_t t = 0; t < left; ++t)
                    result = mul_small(result, p);
            }
        }
        /* Remaining factor, if any, is prime. */
        if (tmp > 1) {
            size_t idx = find_prime_index(tmp, primes, prime_cnt);
            if (idx < prime_cnt) {
                if (rem[idx]) {
                    --rem[idx];
                } else {
                    result = mul_small(result, tmp);
                }
            } else {
                /* tmp is a prime larger than the sieve limit (should not happen). */
                result = mul_small(result, tmp);
            }
        }
    }

    /* Clean up. */
    free(primes);
    free(rem);
    return result;
}
