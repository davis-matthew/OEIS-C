/*
 * A135778  Numbers having number of divisors equal to number of digits in
 *          base 8.
 * https://oeis.org/A135778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer k such that * divisor_count(k)
 *            == digit_count_base8(k)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/digits_base8.h"

static uint32_t primes[3000];
static size_t primes_len = 0;
static uint32_t prime_limit = 0;

/* Ensure primes up to at least sqrt(max_val). */
static void
ensure_primes(uint64_t max_val)
{
    uint32_t lim = (uint32_t)(sqrt((double)max_val) + 1);
    if (lim <= prime_limit)
        return;
    for (uint32_t cand = (prime_limit ? prime_limit + 1 : 2);
         cand <= lim; ++cand)
    {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            if (primes_len < sizeof(primes) / sizeof(primes[0]))
                primes[primes_len++] = cand;
        }
    }
    prime_limit = lim;
}

/* Divisor count for a 64‑bit integer. */
static seq_t
divisor_count(uint64_t n)
{
    uint64_t temp = n;
    uint16_t result = 1;
    for (size_t i = 0; i < primes_len; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp)
            break;
        if (temp % p == 0)
        {
            uint8_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (temp > 1)
        result *= 2;
    return (seq_t)result;
}

/* Dynamic array for caching computed sequence values. */
static seq_t *seq_cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_len = 0;
static uint64_t candidate = 1; /* next integer to test */

/* Append to cache, growing storage if needed. */
static void
append_cache(seq_t val)
{
    if (cache_len == cache_capacity)
    {
        size_t new_cap = cache_capacity == 0 ? 8 : cache_capacity * 2;
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort(); /* In production, handle error. */
        seq_cache = new_ptr;
        cache_capacity = new_cap;
    }
    seq_cache[cache_len++] = val;
}

/* Main function requested by OEIS. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but avoid UB */
    while (cache_len < (size_t)n)
    {
        ensure_primes(candidate);
        seq_t dcnt = divisor_count(candidate);
        int dgt = digits_base8(candidate);
        if (dcnt == dgt)
            append_cache((seq_t)candidate);
        ++candidate;
    }
    return seq_cache[n - 1];
}
