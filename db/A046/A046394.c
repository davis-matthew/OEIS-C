/*
 * A046394  Palindromes with exactly 4 distinct prime factors.
 * https://oeis.org/A046394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th palindrome in increasing order that is
 *            square‑free and has exactly 4 distinct prime factors
 * Range:     u8 none, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/count_prime_factors.h"

/* Build a palindrome from the first half (half) and total length (len). */
static seq_t
make_palindrome(uint64_t half, size_t len)
{
    seq_t res = half;
    seq_t tmp = half;
    if (len & 1U)  /* odd length: skip the middle digit when mirroring */
        tmp /= 10;
    while (tmp) {
        res = res * 10 + (tmp % 10);
        tmp /= 10;
    }
    return res;
}

/* Generate the next palindrome in increasing numerical order. */
static seq_t
next_palindrome(void)
{
    static size_t current_len = 1;         /* current palindrome length */
    static uint64_t half_start = 1;        /* first half for current length */
    static uint64_t half_end   = 9;        /* last half for current length  */
    static uint64_t half        = 1;        /* current half being used */

    while (1) {
        seq_t pal = make_palindrome(half, current_len);

        /* Advance to the next half for the current length. */
        if (++half <= half_end)
            return pal;

        /* Move to the next length. */
        ++current_len;
        size_t half_digits = (current_len + 1) / 2;
        uint64_t start = 1;
        for (size_t i = 1; i < half_digits; ++i)
            start *= 10;
        half_start = start;
        half_end   = start * 10ULL - 1ULL;
        half = half_start;
    }
}

/* Cache for generated sequence terms. */
static seq_t *cache     = NULL;
static size_t cache_sz  = 0;
static size_t cache_cap = 0;

/* Ensure the cache has space for one more element. */
static void
ensure_cache_space(void)
{
    if (cache_sz < cache_cap)
        return;
    cache_cap = cache_cap ? cache_cap << 1 : 128;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!cache)
        exit(EXIT_FAILURE);
}

/* Populate the cache until it contains at least n terms. */
static void
generate_next_term(void)
{
    while (1) {
        seq_t pal = next_palindrome();
        /* Use a uint64_t value for the prime–factor tests.  The
           sequence values that fit in seq_t for n ≤ 10000 are all
           ≤ 2⁶⁴‑1, so the cast is safe.  */
        uint64_t v = (uint64_t)pal;
        if (count_distinct_primes(v) == 4 && count_prime_factors(pal) == 4) {
            ensure_cache_space();
            cache[cache_sz++] = pal;
            return;
        }
    }
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence is 1‑based; this case is undefined */
    while (cache_sz < (size_t)n)
        generate_next_term();
    return cache[n - 1];
}
