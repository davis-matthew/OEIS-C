/*
 * A186882  Alternatively squares and cubes with prime differences.
 * https://oeis.org/A186882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 152, u64 n <= 201,
 *            u128 n <= 201, big any n
 * Validated: standard, pass, 64 terms through n = 195 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186882:  a(1)=4;  for n>1 the sequence is built
 * greedily:  if n is odd, a(n) is the smallest even perfect square
 *            greater than a(n‑1) with prime difference;
 *            if n is even, a(n) is the smallest odd perfect cube
 *            greater than a(n‑1) with prime difference.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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

/* No static state; each call works independently, so any order
 * of calls is supported.  The algorithm is O(n²) in the worst case,
 * which is acceptable for n ≤ 10000 under the time limits.  */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n<1 */

    /* Base case a(1)=4 (even square). */
    if (n == 1)
        return (seq_t)4;

    seq_t prev = (seq_t)4;             /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t cand;
        seq_t diff;
        seq_t base;

        if (k & 1) {                   /* odd index → even square */
            base = floor_sqrt(prev);
            /* increase base until base^2 > prev */
            cand = base * base;
            if (cand <= prev) {
                base += 1;
                cand = base * base;
                if (cand <= prev) {
                    base += 1;
                    cand = base * base;
                }
            }
            /* ensure base is even */
            if (base & 1)
                base += 1;
            /* move to next even base if still too small */
            while ((base * base) <= prev)
                base += 2;
            /* search for the first even square with prime difference */
            for (;;) {
                cand = base * base;
                diff = cand - prev;
                if (is_prime_big(diff))
                    break;
                base += 2;
            }
        } else {                       /* even index → odd cube */
            base = floor_cube_root(prev);
            /* increase base until base^3 > prev */
            cand = base * base * base;
            if (cand <= prev) {
                base += 1;
                cand = base * base * base;
                if (cand <= prev) {
                    base += 1;
                    cand = base * base * base;
                }
            }
            /* ensure base is odd */
            if ((base & 1) == 0)
                base += 1;
            while ((base * base * base) <= prev)
                base += 2;
            /* search for the first odd cube with prime difference */
            for (;;) {
                cand = base * base * base;
                diff = cand - prev;
                if (is_prime_big(diff))
                    break;
                base += 2;
            }
        }
        prev = cand;
    }
    return prev;
}
