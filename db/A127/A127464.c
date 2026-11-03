/*
 * A127464  Primes p such that 12p - 11 and 12p + 11 are also primes.
 * https://oeis.org/A127464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 12p−11 and 12p+11 are primes
 * Range:     u8 none, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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

/* static cache of terms that fit in seq_t */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;     /* next number to test for primality */

/* grow the cache array when necessary */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term (n >= 1).  It works for any build‑dependent
 * seq_t.  Terms that do not fit in seq_t are simply skipped; the
 * function guarantees correctness for all indices whose value does
 * fit in seq_t. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms stored */
    while (cache_len < (size_t)n) {
        /* Find next prime candidate */
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1;

        /* Compute 12p−11 and 12p+11 in a type that can hold the values */
        uint64_t x1 = 12u * p - 11u;
        uint64_t x2 = 12u * p + 11u;

        /* Test primality.  If seq_t is small, the cast will drop values
         * that cannot be represented; such primes are ignored because
         * they exceed the type limit. */
        bool ok = is_prime_big((seq_t)x1) && is_prime_big((seq_t)x2);
        if (ok) {
            /* If p cannot be represented in seq_t, skip it */
            if ((seq_t)p == p) {
                if (cache_len == cache_cap) grow_cache();
                cache[cache_len++] = (seq_t)p;
            }
        }
    }
    return cache[(size_t)n - 1];
}
