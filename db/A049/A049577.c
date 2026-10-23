/*
 * A049577  Primes p such that x^45 = 2 has a solution mod p.
 * https://oeis.org/A049577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049577: primes p such that the congruence x^45 ≡ 2 has a solution mod p.
   The function generates primes in increasing order and keeps those satisfying
   the condition in a memoized cache. When the cache contains at least n
   terms it returns the n‑th term (1‑based indexing). */

/* The condition for a prime p is: 2^((p-1)/gcd(45,p-1)) ≡ 1 (mod p). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* Cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* Candidate start for the next prime search */
static uint64_t prime_start = 2;

/* Utility: Euclidean GCD for uint64_t */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Ensure the cache array can hold at least 'len' elements. */
static void ensure_cache_capacity(size_t len)
{
    if (len <= cache_cap)
        return;
    size_t new_cap = cache_cap < 4 ? 4 : cache_cap * 2;
    while (new_cap < len)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a given prime p satisfies the 45‑th power residue condition. */
static bool qualifies_prime(uint64_t p)
{
    if (p == 2)
        return true;
    uint64_t g = gcd_uint64(45u, p - 1);
    uint64_t exp = (p - 1) / g;
    seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);
    return r == (seq_t)1;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n ≤ 0 we return 0 as a safe value. */
    if (n == 0)
        return (seq_t)0;

    size_t target = (size_t)n;

    /* Generate terms until we have at least 'target' values */
    while (cache_cnt < target) {
        /* Find the next prime starting from prime_start */
        uint64_t p = prime_start;

        /* Handle the first prime (2) specially, then skip even numbers */
        if (p <= 2) {
            if (p == 2) {
                prime_start = 3;
            } else {
                p = 2;
                prime_start = 3;
            }
        } else {
            if (p % 2 == 0)
                p++;
            /* Search for the next prime */
            while (!is_prime((idx_t)p)) {
                p += 2;
            }
            prime_start = p + 2;   /* next odd candidate */
        }

        /* Now 'p' is a prime */
        if (qualifies_prime(p)) {
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt] = (seq_t)p;
            cache_cnt++;
        }
    }

    return cache[target - 1];
}
