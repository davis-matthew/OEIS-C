/*
 * A094320  Numbers n >= 0 such that Lehmer's polynomial 263*n^2+3 is prime.
 * https://oeis.org/A094320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 315, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 7917 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 263*n^2 + 3 is prime. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static seq_t next_candidate = 0;

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = mem_resize(seq_t, cache, new_len);
        for (size_t i = cache_len; i < new_len; ++i) {
            while (1) {
                seq_t val = (seq_t)263 * next_candidate * next_candidate + (seq_t)3;
                if (isPrimeGeneric(val))
                    break;
                ++next_candidate;
            }
            cache[i] = next_candidate;
            ++next_candidate;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
