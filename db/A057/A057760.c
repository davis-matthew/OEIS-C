/*
 * A057760  Numbers n such that 2 is a cube mod n.
 * https://oeis.org/A057760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer n such that (i) n is not
 *            divisible by 4 or 9, and (ii) every prime divisor p of n
 *            satisfies that 2 is a cube mod p. The set of such primes is
 *            A040028
 * Range:     u8 n <= 100, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow_local.h"

/* return 1 if p is a prime belonging to A040028, 0 otherwise.
   For p = 2 or 3 the condition is trivially true.
   For a prime p > 3:
     - if p ≡ 2 (mod 3) then every element of (ℤ/pℤ)× has a cube root,
       so 2 is always a cube;
     - if p ≡ 1 (mod 3) then 2 is a cube iff 2^((p‑1)/3) ≡ 1 (mod p). */
static int good_prime(idx_t p)
{
    if (p == 2 || p == 3)
        return 1;
    if (p % 3 != 1)
        return 1;                /* p ≡ 2 (mod 3) */
    return mod_pow_local(2, (p - 1) / 3, p) == 1;
}

/* test whether integer x satisfies the two conditions of the sequence.
   The function is tolerant of x = 1 (which is accepted).  All arithmetic
   below is performed using 64‑bit unsigned integers; this is sufficient up
   to the 10⁴‑th term. */
static int good_number(idx_t x)
{
    if (x == 1)
        return 1;

    /* reject powers of 2 or 3 of exponent > 1 */
    if (x % 4 == 0 || x % 9 == 0)
        return 0;

    idx_t t = x;

    /* extract powers of 2 */
    while ((t & 1) == 0) {
        t >>= 1;
        /* already ensured exponent <= 1 above */
    }

    /* extract powers of 3 */
    while (t % 3 == 0) {
        t /= 3;
    }

    /* trial division for the remaining part.
       All primes > 3 are either 6k+1 or 6k+5, so we check p and p+2
       for each p = 6k+1. */
    for (uint64_t p = 5; p <= t / p; p += 6) {
        uint64_t cnt = 0;
        while (t % p == 0) {
            t /= p;
            cnt++;
        }
        if (cnt)
            if (!good_prime(p))
                return 0;

        uint64_t p2 = p + 2;
        cnt = 0;
        while (t % p2 == 0) {
            t /= p2;
            cnt++;
        }
        if (cnt)
            if (!good_prime(p2))
                return 0;
    }

    /* if anything remains, it is a prime factor > sqrt(original x) */
    if (t > 1 && !good_prime(t))
        return 0;

    return 1;
}

/* small cache of the sequence terms (stored as uint64_t) */
static uint64_t *cache_vals = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static void push_cache(uint64_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        uint64_t *new_mem = (uint64_t *)realloc(cache_vals, new_cap * sizeof(uint64_t));
        if (!new_mem) { /* malloc failure – abort silently */
            exit(EXIT_FAILURE);
        }
        cache_vals = new_mem;
        cache_cap = new_cap;
    }
    cache_vals[cache_len++] = val;
}

/* Public API: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined – defensive */

    while (cache_len < n) {
        if (good_number(next_candidate))
            push_cache((uint64_t)next_candidate);
        next_candidate++;
    }
    return (seq_t)cache_vals[n - 1];
}
