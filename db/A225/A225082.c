/*
 * A225082  Centrally deletable primes.
 * https://oeis.org/A225082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9753 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distributed computation of OEIS A225082 (centrally deletable primes).
   The function incrementally finds primes, tests central‑digit deletion,
   and memoises results so calls in any order are fast. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Helper: test a 64‑bit integer for primality using the supplied big‑int routine. */
static inline bool is_prime_uint64(uint64_t x)
{
    return is_prime_big((seq_t)x);
}

/* Return true if prime p (as uint64_t) stays prime after deleting its central digit(s). */
static bool is_centrally_deletable(uint64_t p)
{
    if (p < 10)          /* single‑digit primes cannot be deleted */
        return false;

    /* Extract digits into a buffer (least significant first). */
    uint8_t rev[32];
    int d = 0;
    uint64_t t = p;
    while (t > 0 && d < 32) {
        rev[d++] = (uint8_t)(t % 10);
        t /= 10;
    }
    if (d < 2)           /* need at least two digits */
        return false;

    /* Digits in most‑significant order. */
    uint8_t dig[32];
    for (int i = 0; i < d; ++i)
        dig[i] = rev[d - 1 - i];

    /* Indices of digits to skip. */
    int skip1 = -1, skip2 = -1;
    if (d & 1) {                    /* odd length – delete one middle digit */
        skip1 = d / 2;
    } else {                        /* even length – delete two middle digits */
        skip1 = d / 2 - 1;
        skip2 = d / 2;
    }

    /* Reconstruct the number without the middle digit(s). */
    uint64_t res = 0;
    for (int i = 0; i < d; ++i) {
        if (i == skip1 || i == skip2)
            continue;
        res = res * 10 + dig[i];
    }

    if (res == 0)            /* deletion removed all digits (e.g. 2‑digit primes) */
        return false;
    return is_prime_uint64(res);
}

/* --- memoised sequence construction --- */
static seq_t *cache = NULL;           /* array of cached terms */
static size_t cache_len = 0;          /* number of terms stored so far */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Grow cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Skip non‑primes */
        while (!is_prime_uint64(next_candidate))
            ++next_candidate;

        /* If the prime is centrally deletable, store it. */
        if (is_centrally_deletable(next_candidate)) {
            seq_t *tmp = mem_resize(seq_t, cache, (cache_len + 1));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
