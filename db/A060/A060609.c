/*
 * A060609  Repeatedly apply Euler phi to n-th prime; a(n) = highest power of
 *          2 that is seen.
 * https://oeis.org/A060609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A049116(A000040(n))
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime64.h"

static idx_t *primes = NULL;
static size_t prime_count = 0;
static size_t primes_alloc = 0;
static uint64_t last_candidate = 1;

/* Ensure the list of primes contains at least n elements.
   primes[0] is the 1st prime, primes[1] the 2nd, etc. */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    /* Allocate or grow the array if necessary. */
    if (primes_alloc < n) {
        size_t new_alloc = primes_alloc ? primes_alloc << 1 : 64;
        if (new_alloc < n)
            new_alloc = n;
        idx_t *p = (idx_t *)realloc(primes, new_alloc * sizeof(idx_t));
        if (!p) abort();
        primes = p;
        primes_alloc = new_alloc;
    }

    /* Find further primes until we have n of them. */
    while (prime_count < n) {
        ++last_candidate;
        if (!is_prime64(last_candidate))
            continue;
        primes[prime_count++] = last_candidate;
    }
}

/* Compute Euler's totient for a 64‑bit integer by trial division. */
static uint64_t
phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }
    if (m > 1)
        result -= result / m;
    return result;
}

/* Return a(n) = highest power of 2 seen in the Euler‑phi iteration
   chain of the n‑th prime (offset 1). */
seq_t
a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, return 0 as a safe placeholder */
        return 0;

    ensure_primes(n);

    uint64_t cur = primes[n - 1];
    uint64_t max_val = (cur & (cur - 1)) == 0 ? cur : 0;   /* power of 2 check */

    while (cur != 1) {
        cur = phi_uint64(cur);
        if ((cur & (cur - 1)) == 0 && cur > max_val)
            max_val = cur;
    }

    return (seq_t)max_val;
}
