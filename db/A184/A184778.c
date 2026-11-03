/*
 * A184778  Numbers k such that 2k + floor(k*sqrt(2)) is prime.
 * https://oeis.org/A184778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 2*k + floor(k*sqrt(2)) is prime, listed in
 *            increasing order of k (OEIS offset 1)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 1944, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9023 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;        /* stored k values (as seq_t)         */
    static size_t capacity = 0;        /* current allocation capacity       */
    static size_t count = 0;           /* number of terms currently stored  */
    static idx_t last_k = 0;           /* largest k that has been examined  */

    /* initialise storage */
    if (cache == NULL) {
        capacity = 1024;
        cache = mem_new(seq_t, capacity);
    }

    /* generate terms until we have at least n of them */
    while (count < n) {
        idx_t k = last_k + 1;          /* next k to test */
        last_k = k;

        seq_t kk = (seq_t)k;           /* k as seq_t */
        seq_t sq = kk * kk;            /* k^2          */
        sq = sq * 2;                   /* 2*k^2        */

        seq_t floork = floor_sqrt(sq); /* floor(k*sqrt(2)) == floor(sqrt(2*k^2)) */
        seq_t expr  = (seq_t)2 * kk + floork; /* 2*k + floor(k*sqrt(2)) */

        if (is_prime_big(expr)) {
            /* grow cache if necessary */
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = kk;        /* store the qualifying k */
        }
    }

    return cache[n-1];   /* a(1) is stored at index 0 */
}
