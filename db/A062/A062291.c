/*
 * A062291  Primes p = prime(k) such that k*p - 1 is also a prime.
 * https://oeis.org/A062291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th prime p for which the index k of p satisfies k *
 *            p – 1 is also prime. The sequence is indexed starting with a(1)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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

static seq_t cache[200000];
static size_t cache_len = 0;

/* current_prime holds the candidate prime being examined;
   current_index is its index among all primes (starting at 1). */
static uint64_t current_prime = 2;
static idx_t    current_index = 1;

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        /* Test current_prime; if it is prime we consider it a candidate. */
        if (is_prime(current_prime)) {
            /* k*p – 1 */
            seq_t test = (seq_t)current_index * (seq_t)current_prime - 1;
            if (is_prime_big(test)) {
                cache[cache_len++] = current_prime;
            }
            ++current_index;  /* advance the prime index after a prime */
        }

        /* Advance to the next odd number (next prime candidate). */
        if (current_prime == 2) {
            current_prime = 3;
        } else {
            current_prime += 2;
        }
    }

    return cache[(size_t)n - 1];
}
