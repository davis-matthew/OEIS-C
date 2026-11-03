/*
 * A074174  Smallest number k such that k and k+n are of the form p^2*q where
 *          p and q are primes.
 * https://oeis.org/A074174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k and k+n are both of the form p^2*q
 *            where p and q are primes and p != q. The computation searches k
 *            incrementally and memoises results
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 944 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ---------- prime table ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure we have all primes up to at least “limit”. */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit)
        return;

    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* ---------- test whether x is of the form p^2*q with p!=q ---------- */
static bool is_form(seq_t x)
{
    /* The smallest such number is 8 = 2^3, but we disallow p==q. */
    if (x < 8)
        return false;

    /* sqrt(x) limits the possible p. */
    seq_t lim = floor_sqrt(x);
    idx_t sqrt_lim = (idx_t)lim;
    if ((seq_t)sqrt_lim * (seq_t)sqrt_lim < x)
        sqrt_lim++;                      /* round up to be safe */

    ensure_primes(sqrt_lim + 1);

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if ((seq_t)p > sqrt_lim)
            break;
        if (x % (seq_t)p != 0)
            continue;

        seq_t p2 = (seq_t)p * (seq_t)p;
        if (x % p2 != 0)
            continue;

        seq_t m = x / p2;
        if (m <= 1)
            continue;
        if ((seq_t)p == m)               /* p and q must be distinct */
            continue;
        return is_prime_big(m);
    }
    return false;
}

/* ---------- memoisation of a(n) ---------- */
static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;

static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_len)
        return;

    idx_t new_len = n + 1;                /* 1‑based indexing; 0 unused */
    a_cache = mem_resize(seq_t, a_cache, new_len);
    for (idx_t i = a_cache_len; i < new_len; ++i)
        a_cache[i] = 0;
    a_cache_len = new_len;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1 */
        return 0;

    ensure_a_cache(n);
    if (a_cache[n] != 0)
        return a_cache[n];

    seq_t k = 1;
    for (;;) {
        if (is_form(k) && is_form(k + n)) {
            a_cache[n] = k;
            return k;
        }
        ++k;
    }
}
