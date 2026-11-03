/*
 * A072360  One-sixth the area of the smallest primitive d-arithmetic
 *          triangle, where d=A072330(n).
 * https://oeis.org/A072360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x'*y'/2 where (x',y') is the minimal primitive solution
 *            to * x^2 - 3*y^2 = d^2, with d = A072330(n). d is the * n-th
 *            integer all of whose prime factors are congruent to * 1 or 11
 *            (mod 12) and not divisible by 3. This function * generates terms
 *            on demand
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 929 (2026-11-03)
 * Helpers:   gcd, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

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

static bool d_is_allowed(idx_t d)
{
    /* d must not be divisible by 3 and
     * each prime factor must be ≡ 1 or 11 (mod 12). */
    if (d % 3 == 0) return false;
    idx_t tmp = d;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            if (p % 12 != 1 && p % 12 != 11) return false;
        }
    }
    if (tmp > 1 && (tmp % 12 != 1 && tmp % 12 != 11)) return false;
    return true;
}

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_alloc = 0;
static idx_t next_d = 1;

static void grow_cache(size_t needed)
{
    if (cache_alloc < needed) {
        size_t new_cap = cache_alloc ? cache_alloc * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
        cache = new_ptr;
        cache_alloc = new_cap;
    }
}

static seq_t compute_area_for_d(idx_t d)
{
    seq_t dval = d;
    for (idx_t y = 1; y <= d; ++y) {
        seq_t lhs = dval * dval + 3 * (seq_t)y * (seq_t)y;
        seq_t x = floor_sqrt(lhs);
        if (x * x == lhs && gcd(x, (seq_t)y) == 1) {
            return x * y / 2;
        }
    }
    /* Should never reach here for allowed d. */
    return 0;
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms. */
    while (cache_size < n) {
        /* Find next allowed d. */
        idx_t d;
        for (;;) {
            d = next_d++;
            if (!d_is_allowed(d)) continue;
            seq_t area = compute_area_for_d(d);
            /* area will be non-zero for allowed d. */
            if (area > 0) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = area;
                break;
            }
        }
    }
    return cache[n - 1];
}
