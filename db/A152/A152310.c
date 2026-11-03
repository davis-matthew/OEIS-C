/*
 * A152310  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/10.
 * https://oeis.org/A152310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 961 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that ord_p(2) = (p-1)/10 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Check whether a prime p satisfies ord_p(2) = (p-1)/10.
   p is assumed to be 1 (mod 10). */
static bool has_tenth_order_prime(seq_t p)
{
    /* (p-1)/10 fits within uint64_t for the ranges we consider. */
    uint64_t m = (uint64_t)((p - (seq_t)1) / (seq_t)10);

    /* 2^m ≡ 1 (mod p) is necessary. */
    if (powmod((seq_t)2, m, p) != (seq_t)1)
        return false;

    /* Test each distinct prime factor q of m.
       If 2^(m/q) ≡ 1 (mod p), the order would divide m/q, not m. */
    uint64_t tmp = m;
    for (uint64_t q = 2; q * q <= tmp; ++q) {
        if (tmp % q == 0) {
            /* q is a prime factor of m. */
            if (powmod((seq_t)2, m / q, p) == (seq_t)1)
                return false;
            /* Skip all powers of q. */
            while (tmp % q == 0)
                tmp /= q;
        }
    }
    if (tmp > 1) {                /* remaining prime factor > sqrt(m) */
        if (powmod((seq_t)2, m / tmp, p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Return the n-th prime satisfying the order condition. */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms          */
    static size_t cache_cap = 0;   /* capacity of the allocation       */

    /* Grow the cache until it contains at least n terms. */
    if (cache_len < (size_t)n) {
        /* Ensure cache has some non‑zero capacity to start with. */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = mem_new(seq_t, cache_cap);
        }

        /* Current candidate.  We step by 10 to stay 1 (mod 10). */
        seq_t candidate = 151;
        if (cache_len > 0)
            candidate = cache[cache_len - 1] + (seq_t)10;

        /* Continue searching until we have enough terms. */
        while (cache_len < (size_t)n) {
            /* Test primality. */
            if (is_prime_big(candidate)) {
                /* Candidate is prime; now test the order condition. */
                if (has_tenth_order_prime(candidate)) {
                    /* Grow cache if necessary. */
                    if (cache_len == cache_cap) {
                        cache_cap *= 2;
                        cache = mem_resize(seq_t, cache, cache_cap);
                    }
                    cache[cache_len++] = candidate;
                }
            }
            /* Next candidate congruent to 1 modulo 10. */
            candidate += (seq_t)10;
        }
    }
    return cache[(size_t)n - 1];
}
