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
 * Validated: standard, pass, 64 terms through n = 943 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc, free */
#include <stdbool.h>  /* bool */
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
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
    seq_t r = powmod((seq_t)7, exp,
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
