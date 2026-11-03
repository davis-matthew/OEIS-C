/*
 * A123317  Smallest prime power m such that n+m is a prime number.
 * https://oeis.org/A123317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Validated: partial, pass, 102 terms through n = 102 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A123317: smallest prime power m such that n + m is prime.
   The function scans m = 1,2,3,…, returning the first prime power
   that makes n+m prime.  Prime powers are recognised by trial division
   on a pre‑computed list of primes up to 100 000, which covers all
   m that occur for n ≤ 10000 in under the 2‑second limit.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
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

/* Global storage for the prime list. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise the prime list once. */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    /* 100 000 is comfortably larger than sqrt(max m) expected for n ≤ 10 000. */
    build_primes_upto(100000, &prime_list, &prime_count);
}

/* Test whether a uint64_t value is a prime power (1, p, p², …). */
static bool is_prime_power_uint64(uint64_t m)
{
    if (m == 1)          /* 1 is allowed as a prime power. */
        return true;

    uint64_t temp = m;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > temp / p)    /* p² > temp */
            break;
        if (temp % p == 0) {
            /* Divide out all factors of p. */
            while (temp % p == 0)
                temp /= p;
            /* We are a prime power iff nothing else remains. */
            return temp == 1;
        }
    }
    /* No small prime factor found – m is prime itself. */
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    /* Search for the smallest prime power m with n+m prime. */
    for (uint64_t m = 1; ; ++m) {
        if (!is_prime_power_uint64(m))
            continue;

        seq_t candidate = (seq_t)n + (seq_t)m;
        if (is_prime_big(candidate))
            return (seq_t)m;   /* m fits in seq_t because m is small. */
    }
}
