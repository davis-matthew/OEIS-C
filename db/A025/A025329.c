/*
 * A025329  Numbers that are the sum of 3 nonzero squares in exactly 9 ways.
 * https://oeis.org/A025329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 400, u32 n <= 400, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that are the sum of three nonzero squares in exactly 9 unordered ways */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: integer square root of a 64‑bit unsigned integer */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            idx_t sq = mid * mid;
            if (sq == n) return mid;
            if (sq < n) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return high;
}

/* Count unordered triples (a,b,c) with a,b,c >= 1 and a <= b <= c satisfying
 * a^2 + b^2 + c^2 == n. */
static int count_three_squares_unordered_nonzero(idx_t n)
{
    int count = 0;
    for (idx_t a = 1; a * a <= n; ++a) {
        idx_t a2 = a * a;
        if (a2 >= n) break;
        idx_t rem_ab = n - a2;
        for (idx_t b = a; b * b <= rem_ab; ++b) {
            idx_t b2 = b * b;
            idx_t rem = rem_ab - b2;
            if (rem < b2) break;        /* ensures c >= b */
            idx_t c = isqrt_u64(rem);
            if (c * c == rem && c >= b) ++count;
        }
    }
    return count;
}

/* Dynamic array helpers for caching results */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t last_checked = 0;

static void ensure_capacity(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value for index 0 is irrelevant */
    if (n <= cache_len) return cache[n - 1];

    /* Extend the cache until we have at least n terms */
    for (idx_t num = last_checked + 1; ; ++num) {
        if (count_three_squares_unordered_nonzero(num) == 9) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)num;
        }
        if (cache_len >= n) {
            last_checked = num;
            break;
        }
    }
    return cache[n - 1];
}
