/*
 * A090736  Number of positive integers <= n that can be expressed as a sum of
 *          2 coprime squares > 0.
 * https://oeis.org/A090736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers ≤ n that can be expressed as a sum of
 *            two coprime * squares > 0. The sequence is computed by marking
 *            all n that are representable * by a² + b² with gcd(a,b)=1
 *            (a,b≥1) and prefix‑summing the marks
 * Range:     u8 n <= 1384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Small helpers that avoid the external library functions      */
static idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t _isqrt(idx_t n) {
    idx_t low = 0, high = (n >> 1) + 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}
/* ------------------------------------------------------------ */

/* Static cache holders – kept between calls so later requests are fast. */
static idx_t   max_cached   = 0;   /* highest n that has been processed */
static uint8_t *rep_mask    = NULL;/* 0/1 flag for each representable integer */
static idx_t   *cum_count   = NULL;/* cumulative counts, stored in idx_t */

/* Extend the cache so that it contains all numbers up to at least n. */
static void extend_cache(idx_t n)
{
    if (n <= max_cached) return;            /* nothing to do */

    /* Resize or allocate the mask array */
    if (!rep_mask) {
        rep_mask = (uint8_t *)calloc(n + 1, sizeof(uint8_t));
        cum_count = (idx_t *)calloc(n + 1, sizeof(idx_t));
    } else {
        rep_mask  = (uint8_t *)realloc(rep_mask,  (n + 1) * sizeof(uint8_t));
        cum_count = (idx_t *)realloc(cum_count, (n + 1) * sizeof(idx_t));
        for (idx_t i = max_cached + 1; i <= n; ++i) rep_mask[i] = 0;
    }

    /* Mark all numbers that can be written as a² + b² with coprime a,b ≥ 1. */
    idx_t lim = _isqrt(n);
    for (idx_t a = 1; a <= lim; ++a) {
        for (idx_t b = a; b <= lim; ++b) {
            idx_t s = a * a + b * b;
            if (s > n) break;
            if (_gcd_idx(a, b) == 1) rep_mask[s] = 1;
        }
    }

    /* Build cumulative counts for the new range. */
    cum_count[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        cum_count[i] = cum_count[i - 1] + (rep_mask[i] ? (idx_t)1 : (idx_t)0);

    max_cached = n;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is defined from n=1 onward */
        return (seq_t)0;

    extend_cache(n);
    return (seq_t)cum_count[n];
}
