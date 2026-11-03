/*
 * A142252  Primes congruent to 3 mod 43.
 * https://oeis.org/A142252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 1000 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A142252: Primes congruent to 3 mod 43
 * a(n) = nth prime p such that p mod 43 == 3
 * The function performs an efficient on‑demand search and stores
 * the primes found in a static cache.  The cache grows to a
 * sufficient size to accommodate the expected range (up to ≈10 000
 * terms).  The implementation uses the supplied is_prime_big
 * routine for primality testing, which works for all builds.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Maximum number of primes we expect to store (≈10 000 terms). */
#define PRIME_CAP 50000

static seq_t prime_cache[PRIME_CAP];
static size_t cache_used = 0;      /* number of primes currently cached */
static uint64_t next_candidate = 3;/* next number ≡3 (mod 43) to test */

/* Return the n-th prime p ≡ 3 (mod 43) (n ≥ 1).  Calls are thread‑unsafe. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0; harmless */

    /* Convert n to an unsigned index for array access. */
    size_t target = (size_t)n;

    /* Ensure the cache contains at least 'target' elements. */
    while (cache_used < target) {
        if (cache_used == PRIME_CAP) {
            /* Should not happen for the intended range. */
            abort();
        }

        seq_t candidate = (seq_t)next_candidate;
        if (is_prime_big(candidate))
            prime_cache[cache_used++] = candidate;

        /* Advance to the next candidate congruent to 3 modulo 43. */
        next_candidate += 43;
    }

    /* Return the cached prime (array indexes are 0‑based). */
    return prime_cache[target - 1];
}
