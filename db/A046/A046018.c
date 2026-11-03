/*
 * A046018  Discriminants of imaginary quadratic fields with class number 21
 *          (negated).
 * https://oeis.org/A046018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: full, pass, 85 terms through n = 85 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method:  Enumerate fundamental discriminants d>0, compute their class
   numbers by counting reduced binary quadratic forms (class number 21),
   collect the first 85 such discriminants (output section) */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
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

static bool _squarefree(uint64_t n) {
    if (n % 4 == 0) return false;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % (p * p) == 0) return false;
    }
    return true;
}

static seq_t _classnumber(long long D) {
    /* D is negative */
    seq_t absD = (seq_t)(-D);
    seq_t tmp   = absD / 3;
    seq_t max_a_seq = floor_sqrt(tmp);
    uint64_t max_a = (uint64_t)max_a_seq;
    seq_t h = 0;
    for (uint64_t a = 1; a <= max_a; ++a) {
        for (int64_t b = -(int64_t)a; b <= (int64_t)a; ++b) {
            int64_t lhs = (int64_t)b * (int64_t)b - D;
            if (lhs % (4 * a) != 0) continue;
            int64_t c = lhs / (4 * a);
            if (a > c) continue;
            if ((a == c || (b < 0 && (b * b) == (long long)a * a) || (b < 0 && llabs(b) == a)) && b < 0)
                continue;
            if (a == c || (llabs(b) == a)) {
                if (b < 0) continue;
            }
            h++;
        }
    }
    return h;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool cache_ready = false;

seq_t a(idx_t n) {
    if (!cache_ready) {
        const size_t TARGET = 85;
        cache = alloc_seq(TARGET);
        size_t count = 0;
        for (uint64_t d = 3; ; ++d) {
            if (d % 4 != 3 && d % 4 != 0) continue;   /* not a possible fundamental discriminant */
            if (d % 4 == 3) {
                if (!_squarefree(d)) continue;
            } else { /* d % 4 == 0 */
                uint64_t m = d / 4;
                if (!(m % 4 == 1 || m % 4 == 2)) continue;
                if (!_squarefree(m)) continue;
            }
            long long D = -(long long)d;
            seq_t h = _classnumber(D);
            if (h == 21) {
                cache[count++] = d;
                if (count >= TARGET) break;
            }
        }
        cache_len = count;
        cache_ready = true;
    }
    if (n == 0 || (size_t)n > cache_len) return 0;
    return cache[(size_t)(n - 1)];
}
