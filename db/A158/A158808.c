/*
 * A158808  Denominator of Bernoulli(n, 1/9).
 * https://oeis.org/A158808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 3^(2n) · 2^e(n) · ∏{p prime, p>3, (p-1)|n} p, where e(n)
 *            = 1 if n is even or n==1, otherwise e(n)=0. This is the
 *            denominator of the Bernoulli polynomial B_n(1/9)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Static cache for primes */
static idx_t _prime_limit = 0;
static uint64_t *_prime_cache = NULL;
static size_t _prime_count = 0;

/* Build primes up to 'limit' (inclusive) using a simple sieve.
   The list is stored in _prime_cache and _prime_count. */
static void _build_primes(idx_t limit)
{
    if (limit <= _prime_limit)
        return; /* already built */

    size_t sz = (size_t)(limit + 1);          /* number of bools needed */
    uint8_t *is_prime = (uint8_t *)calloc(sz, 1); /* 0 = not prime, 1 = potential prime */
    if (!is_prime)
        return; /* allocation failure: caller will treat as missing primes */

    /* mark potential primes */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[(size_t)i] = 1;

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!is_prime[(size_t)i])
            continue;
        for (idx_t j = i * i; j <= limit; j += i)
            is_prime[(size_t)j] = 0;
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* allocate array for primes */
    uint64_t *prime_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_arr) {
        free(is_prime);
        return; /* allocation failure */
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[(size_t)i])
            prime_arr[pos++] = (uint64_t)i;
    }

    free(is_prime);
    free(_prime_cache);   /* free previous array if any */

    _prime_cache   = prime_arr;
    _prime_count   = cnt;
    _prime_limit   = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* base case */

    /* Build prime list up to n+1 if needed */
    _build_primes(n + 1);

    /* Start with 3^(2n) */
    seq_t result = seq_pow((seq_t)3, (idx_t)(2 * n));

    /* Multiply by 2 if n is even or n==1 */
    if ((n % 2) == 0 || n == 1)
        result = seq_mul(result, (seq_t)2);

    /* Multiply by primes p>3 with (p-1)|n */
    for (size_t i = 0; i < _prime_count; ++i) {
        uint64_t p = _prime_cache[i];
        if (p <= 3)          /* skip 2 and 3 */
            continue;
        if (n % (p - 1) == 0)
            result = seq_mul(result, (seq_t)p);
    }

    return result;
}
