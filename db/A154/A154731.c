/*
 * A154731  Integers of the form k = m^3-m^2 such that k-+1 are primes.
 * https://oeis.org/A154731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k of the form m³–m² such that k–1 and
 *            k+1 are primes
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
#ifdef __cplusplus
#include <vector>
#endif

/* Compute k = m³ – m².  The computation uses 128‑bit arithmetic for 64‑bit
   seq_t, otherwise the big‑integer arithmetic of seq_t. */
static seq_t compute_k(uint64_t m)
{
    /* 64‑bit integer builds: use unsigned __int128 for safety. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 t = (unsigned __int128)m * m;
        t = t * (m - 1);
        return (seq_t)t;
    } else {
        seq_t ms = (seq_t)m;
        return ms * ms * (ms - 1);
    }
}

#if !defined(__cplusplus)
/* C build: dynamic array helpers. */
static seq_t *cache_arr = NULL;      /* array of found terms */
static size_t cache_cap = 0;         /* current capacity of cache_arr */
static size_t cache_size = 0;        /* number of stored terms */
static uint64_t next_m_c = 2;        /* next value of m to test */

static void grow_cache_arr(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed) new_cap *= 2;
        seq_t *p = realloc(cache_arr, new_cap * sizeof(seq_t));
        cache_arr = p;
        cache_cap = new_cap;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 – defensive */

#if defined(__cplusplus)
    /* C++ build: use std::vector to store the values. */
    static std::vector<seq_t> cache_vec;
    static uint64_t next_m = 2;
    size_t target = (size_t)n;
    while (cache_vec.size() < target) {
        seq_t k = compute_k(next_m);
        seq_t km1 = k - 1;
        seq_t kp1 = k + 1;
        if (is_prime_big(km1) && is_prime_big(kp1))
            cache_vec.push_back(k);
        ++next_m;
    }
    return cache_vec[target - 1];
#else
    /* C build: use dynamic array. */
    size_t target = (size_t)n;
    while (cache_size < target) {
        seq_t k = compute_k(next_m_c);
        seq_t km1 = k - 1;
        seq_t kp1 = k + 1;
        if (is_prime_big(km1) && is_prime_big(kp1)) {
            grow_cache_arr(cache_size + 1);
            cache_arr[cache_size++] = k;
        }
        ++next_m_c;
    }
    return cache_arr[n - 1];
#endif
}
