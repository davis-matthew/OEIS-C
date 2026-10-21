/*
 * A080732  Smallest distance from n to a prime power (as defined in A246655).
 * https://oeis.org/A080732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = min(|n - k| : k is a prime power) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit values (no floating point) */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > v / mid)   /* avoid overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Prime list utilities ---------------------------------------------- */
static uint64_t *prime_list     = NULL;
static size_t   prime_list_cnt  = 0;
static uint64_t prime_list_limit = 0;

/* Build primes up to 'limit' using a simple sieve of Eratosthenes.  */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_list_limit)
        return;

    size_t sz = (size_t)(limit + 1);
    bool *is_composite = (bool *)calloc(sz, sizeof(bool));
    if (!is_composite)
        return;   /* allocation failure; sequence will be incorrect */

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes and rebuild array */
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++new_cnt;

    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cnt * sizeof(uint64_t));
    if (!new_list) {
        free(is_composite);
        return;   /* allocation failure */
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i])
            new_list[idx++] = i;
    }

    prime_list      = new_list;
    prime_list_cnt  = new_cnt;
    prime_list_limit = limit;
    free(is_composite);
}

/* Test if x is a prime power (including primes itself).  */
static bool is_prime_power(uint64_t x)
{
    if (x < 2)
        return false;

    /* If x itself is prime, it is a prime power. */
    uint64_t sqrt_x = isqrt_u64(x);
    ensure_primes_upto(sqrt_x);

    for (size_t i = 0; i < prime_list_cnt && prime_list[i] <= sqrt_x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            /* Divide out all powers of p */
            while (x % p == 0)
                x /= p;
            return (x == 1);
        }
    }
    /* No divisor found: x is prime. */
    return true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset = 1; behavior for n = 0 is irrelevant. */
    if (n == 0)
        return 0;

    uint64_t u = (uint64_t)n;            /* safe cast (idx_t is unsigned). */
    /* Quick check for the number itself. */
    if (is_prime_power(u))
        return (seq_t)0;

    /* Search outward until a prime power is found. */
    for (uint64_t d = 1; ; ++d) {
        uint64_t low = u > d ? u - d : 0;
        if (low >= 1 && is_prime_power(low))
            return (seq_t)d;

        uint64_t high = u + d;
        if (is_prime_power(high))
            return (seq_t)d;
        /* The loop is guaranteed to terminate because prime powers are infinite
           and arbitrarily close to any integer. */
    }
}
