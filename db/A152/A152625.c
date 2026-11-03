/*
 * A152625  Prime numbers p such that 2p+1, 4p+1, 8p+1, 10p+1, 14p+1 and 16p+1
 *          are all composite numbers.
 * https://oeis.org/A152625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that 2p+1, 4p+1, 8p+1, 10p+1, 14p+1 and
 *            16p+1 are all composite numbers
 * Range:     u8 none, u16 n <= 212, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9948 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* External function required by the build */
seq_t a(idx_t n)
{
    /* CACHE: store found sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* PRIMES: store all primes found so far for trial division */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;

    /* Helper to grow prime array */
    if (primes_cap == 0) {
        primes_cap = 8;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* Ensure cache has enough capacity */
    if (cache_len == 0 && cache_cap == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate sequence terms until we have the nth one */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate */
        static uint64_t cand = 2;   /* start with 2, will be incremented below */
        /* Move to next potential prime */
        if (cand == 2) {
            cand = 3;
        } else {
            cand += 2;  /* only test odd numbers */
        }

        /* Test cand for primality using trial division by existing primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (!is_prime) continue;

        /* Record the prime for future tests */
        if (primes_len == primes_cap) {
            primes_cap *= 2;
            primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_len++] = cand;

        /* Check the required composite conditions */
        seq_t p = (seq_t)cand;

        seq_t e1 = 2 * p + 1;
        seq_t e2 = 4 * p + 1;
        seq_t e3 = 8 * p + 1;
        seq_t e4 = 10 * p + 1;
        seq_t e5 = 14 * p + 1;
        seq_t e6 = 16 * p + 1;

        if (is_prime_big(e1)) continue;
        if (is_prime_big(e2)) continue;
        if (is_prime_big(e3)) continue;
        if (is_prime_big(e4)) continue;
        if (is_prime_big(e5)) continue;
        if (is_prime_big(e6)) continue;

        /* All six numbers are composite: store in cache */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = p;
    }

    /* Return the requested term */
    return cache[n - 1];
}
