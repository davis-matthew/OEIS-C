/*
 * A201998  Positive numbers n such that n^2 + n + 41 is composite and there
 *          are no positive integers c such that n = c*x^2 + (c + 1)*x + c*41
 *          for an integer x.
 * https://oeis.org/A201998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer n such that * d = n^2 + n + 41
 *            is composite and there is no positive integer c * and integer x
 *            for which n = c*x^2 + (c + 1)*x + c*41
 * Range:     u8 none, u16 n <= 3, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *cache      = NULL;   /* allocated array of terms */
    static size_t  cache_cap = 0;      /* current capacity of cache */
    static idx_t   cache_len = 0;      /* number of terms stored */
    static seq_t   next_candidate = 1; /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = next_candidate++;   /* candidate integer to test */

        /* 1. Check that cand^2 + cand + 41 is composite */
        seq_t val = cand * cand + cand + 41;
        if (is_prime_big(val)) {
            continue;            /* val is prime, skip this candidate */
        }

        /* 2. Ensure cand cannot be written as c*x^2 + (c+1)*x + c*41 */
        bool has_rep = false;

        /* 2a.  x >= 0 : n = c*q + x, q = x^2 + x + 41 */
        for (seq_t x = 0; x <= cand; ++x) {
            seq_t q   = x * x + x + 41;
            seq_t lhs = cand - x;          /* cand - x is safe because x <= cand */

            if (q > lhs) break;            /* no further possibilities */
            if ((lhs % q) == 0) {
                has_rep = true;
                break;
            }
        }

        /* 2b.  x < 0 : let y = -x > 0, then n = c*s - y, s = y^2 - y + 41 */
        if (!has_rep) {
            for (seq_t y = 1; ; ++y) {
                seq_t s   = y * y - y + 41;
                seq_t rhs = cand + y;          /* n + y > n */

                if (s > rhs) break;
                if ((rhs % s) == 0) {
                    has_rep = true;
                    break;
                }
            }
        }

        if (!has_rep) {
            /* keep the term in the cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = mem_resize(seq_t, cache, new_cap);
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
