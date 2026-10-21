/*
 * A091282  Exponent of 2 in the prime factorization of prime(n)^2 - 1.
 * https://oeis.org/A091282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 in prime(n)^2 - 1 (OEIS A091282)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the n-th prime by incremental sieving and
   evaluate v2(p-1)+v2(p+1).  The algorithm is O(L log log L) for
   the sieve where L ≈ prime(n).  For n up to 10000 this is far below
   the time limit.  The result fits into seq_t because the exponent is
   small (≤ 20 for primes < 2^20).                                      */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/prime_upper_bound.h"

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cap  = 0;       /* number of stored primes   */

/* Compute the smallest power of 2 dividing x.
   For 64‑bit values, use a loop (portable). */
static uint64_t v2_uint64(uint64_t x)
{
    uint64_t c = 0;
    while ((x & 1ULL) == 0ULL && x != 0ULL) {
        x >>= 1;
        ++c;
    }
    return c;
}

/* Generate a list of the first at least n primes and store it in
   prime_list.  If the list already contains enough primes it does
   nothing.  The sieve size is the upper bound returned by
   prime_upper_bound.  The function reallocates prime_list when
   necessary.*/
static void ensure_primes(idx_t n)
{
    if (prime_cap >= (size_t)n) return;          /* already enough */

    uint64_t limit = prime_upper_bound(n);
    /* Simple sieve of Eratosthenes using a byte array. */
    size_t sz = limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime) return;                     /* allocation failure */

    for (uint64_t i = 2; i < sz; ++i) is_prime[i] = true;
    for (uint64_t p = 2; p * p < sz; ++p) {
        if (is_prime[p]) {
            for (uint64_t q = p * p; q < sz; q += p) is_prime[q] = false;
        }
    }

    /* Count primes to allocate exact array size. */
    size_t count = 0;
    for (uint64_t i = 2; i < sz; ++i) if (is_prime[i]) ++count;
    if (count < (size_t)n) {
        free(is_prime);
        /* In the extremely unlikely event that the bound is too small
           we double it and try again. */
        while (count < (size_t)n) {
            limit *= 2;
            sz = limit + 1;
            bool *old = is_prime;
            is_prime = (bool *)malloc(sz);
            if (!is_prime) { free(old); return; }
            for (uint64_t i = 2; i < sz; ++i) is_prime[i] = true;
            for (uint64_t p = 2; p * p < sz; ++p) {
                if (is_prime[p]) {
                    for (uint64_t q = p * p; q < sz; q += p) is_prime[q] = false;
                }
            }
            free(old);
            count = 0;
            for (uint64_t i = 2; i < sz; ++i) if (is_prime[i]) ++count;
        }
    }

    /* Store primes in dynamic array. */
    uint64_t *new_primes = (uint64_t *)realloc(prime_list, count * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;  /* allocation failure, keep previous list */
    }
    prime_list = new_primes;

    size_t idx = 0;
    for (uint64_t i = 2; i < sz; ++i) if (is_prime[i]) {
        prime_list[idx++] = i;
    }
    prime_cap = count;
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    ensure_primes(n);                    /* make sure we have the n-th prime */

    uint64_t p = prime_list[(size_t)(n - 1)];  /* primes are 0‑based */
    /* For p = 2 the exponent is 0.  For odd p we add exponents of (p-1) and (p+1). */
    seq_t result;
    if (p == 2ULL) {
        result = (seq_t)0;
    } else {
        uint64_t e1 = v2_uint64(p - 1ULL);
        uint64_t e2 = v2_uint64(p + 1ULL);
        result = (seq_t)(e1 + e2);
    }
    return result;
}
