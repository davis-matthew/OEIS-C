/*
 * A031591  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 93.
 * https://oeis.org/A031591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A031591: Numbers k such that the continued fraction for sqrt(k)
 *  has even period and, after deleting the last term of the period,
 *  the central term of the remaining period equals 93.
 *
 *  This implementation searches integers in increasing order,
 *  caching found values so that calls may be made in any order.
 *  No library functions beyond the basic C/C++ headers are used.
 *  Integer square root is computed with an integer binary search
 *  (no floating point or <math.h>).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* --- Auxiliary: integer square root for idx_t (uint64_t) -------------------------------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;   /* high^2 fits in 64 bits */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

/* --- Check whether k satisfies the conditions of A031591 ----------------------------- */
static bool qualifies(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    if (a0 * a0 == k)  /* perfect square */
        return false;

    idx_t m = 0;
    idx_t d_mul = 1;
    idx_t a = a0;
    idx_t terms[2048];
    size_t i = 0;

    for (;;) {
        /* compute next terms of continued fraction of sqrt(k) */
        __uint128_t prod = (__uint128_t)d_mul * a;
        m = (idx_t)(prod - m);

        __uint128_t num = (__uint128_t)k - (__uint128_t)m * m;
        d_mul = (idx_t)(num / d_mul);

        a = (a0 + m) / d_mul;

        if (i >= 2048)  /* safety guard; should never happen */
            return false;
        terms[i++] = a;

        if (a == 2 * a0)  /* periodic part finished */
            break;
    }

    if (i & 1)  /* odd period length -> reject */
        return false;

    /* central term after deleting the last term */
    if (terms[i / 2 - 1] == 93)
        return true;
    return false;
}

/* --- Static cache for found terms --------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t next_candidate = 1;

/* --- Entry point --------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* a(0) undefined */

    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        while (!qualifies(next_candidate))
            ++next_candidate;

        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = (idx_t)new_cap;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
