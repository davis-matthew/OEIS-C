/*
 * A040171  Primes p such that x^5 = 7 has a solution mod p.
 * https://oeis.org/A040171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p for which the congruence x^5 ≡ 7 (mod p)
 *            has a solution
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc, free */
#include <stdbool.h>  /* bool */
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;          /* cached sequence values            */
static size_t cache_cap = 0;         /* current capacity of cache         */
static size_t cache_len = 0;         /* number of elements stored         */
static uint64_t next_candidate = 2;  /* next number to test for primality */

/* Grows the cache so that it can hold at least 'need' elements.        */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)          /* out of memory – callers will abort      */
        return;
    cache = new_arr;
    cache_cap = new_cap;
}

/* Test if prime p satisfies the required property. */
static int good_prime(uint64_t p)
{
    if (p % 5 != 1)
        return 1;                     /* 5 ∤ p−1 → every residue is a 5th power */
    uint64_t exp = (p - 1) / 5;
    seq_t r = mod_pow((seq_t)7, exp,
                      (seq_t)p);        /* 7^((p−1)/5) (mod p) */
    return r == 1;
}

/* Main function required by the specification.                     */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; undefined for n=0 */

    /* Ensure the cache contains at least n elements.                */
    while (cache_len < (size_t)n) {
        grow_cache(cache_len + 1);

        /* Find the next prime p ≥ next_candidate. */
        uint64_t p = next_candidate;
        if (p == 2) {
            /* keep p = 2 */
        } else {
            if (p % 2 == 0)
                p++;                    /* start at odd */
            while (!is_prime_big(p))
                p += 2;
        }

        /* If good, store it. */
        if (good_prime(p))
            cache[cache_len++] = (seq_t)p;

        /* Advance the candidate for the next iteration. */
        if (p == 2)
            next_candidate = 3;
        else
            next_candidate = p + 2;
    }

    return cache[n - 1];
}
