/*
 * A061450  a(1) = 1; a(n+1) = smallest cube > a(n) with leading digit equal
 *          to final digit of a(n) and final digit not 0.
 * https://oeis.org/A061450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   cube, dec_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061450:  a(1) = 1;   a(n+1) = the smallest cube > a(n)
   whose leading decimal digit equals the last decimal digit of a(n)
   and whose last digit is not zero.                  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cube.h"
#include "../utils/dec_digits.h"

/* ---------- helper routines ---------- */

/* floor of the integer cube root of x */
static inline idx_t floor_cuberoot(seq_t x)
{
    idx_t lo = 0, hi = 1;
    /* exponentially find an upper bound */
    while (cube(hi) <= x) hi <<= 1;
    /* binary search */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (cube(mid) <= x) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

/* 10^k as seq_t (k is small – up to a few dozen) */
static inline seq_t pow10(idx_t k)
{
    seq_t r = 1;
    while (k--) r *= 10;
    return r;
}

/* leading decimal digit of x (x>0) */
static inline seq_t leading_digit(seq_t x)
{
    idx_t d = dec_digits(x) - 1;
    seq_t p = pow10(d);
    return x / p;
}

/* ---------- caching --------------------------------------- */

static seq_t  *cache     = NULL;
static idx_t   cache_len = 0;   /* number of terms computed (a(1) at cache[0]) */
static idx_t   cache_cap = 0;   /* allocated slots */

/* make sure cache can hold up to n terms (1‑based indexing) */
static void ensure_cache(idx_t n)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    } else if (n > cache_cap) {
        while (n > cache_cap) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        seq_t last_digit = prev % 10;        /* guaranteed non‑zero */
        idx_t k = floor_cuberoot(prev) + 1;  /* first cube strictly larger */

        for (;;) {
            seq_t cand = cube(k);
            if (cand <= prev) { ++k; continue; }

            if (leading_digit(cand) == last_digit && (cand % 10) != 0) {
                cache[cache_len++] = cand;
                break;
            }
            ++k;
        }
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset – harmless */

    if (cache_len == 0) { /* initialise the first term */
        cache_cap = 16;
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0]  = (seq_t)1;      /* a(1) = 1 */
        cache_len = 1;
    }

    ensure_cache(n);
    return cache[n - 1];
}
