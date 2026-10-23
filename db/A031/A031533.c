/*
 * A031533  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 35.
 * https://oeis.org/A031533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A031533:
 *  Numbers k such that the continued fraction for sqrt(k) has even
 *  period and, after removing the last term, the central term of
 *  the remaining period equals 35. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Integer sqrt via binary search (returns ⌊√x⌋) */
static idx_t floor_sqrt_idx(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 prod = (__int128)mid * mid;
        if (prod <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;
}

/* Vector of period terms (indices start at 0) */
static idx_t *period_vec = NULL;
static size_t period_cap = 0;
static size_t period_len = 0;

/* Test whether an integer k satisfies the definition   */
static bool satisfies(idx_t k)
{
    idx_t a0 = floor_sqrt_idx(k);
    if (a0 * a0 == k)   /* k is a perfect square */
        return false;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    period_len = 0;

    while (true) {
        /* m = d*a - m */
        __int128 tmp = (__int128)d * a - m;
        m = (idx_t)tmp;

        /* d = (k - m*m)/d */
        __int128 m2   = (__int128)m * m;
        __int128 numer = (__int128)k - m2;
        d = (idx_t)(numer / d);

        /* a = (a0 + m) / d */
        a = (idx_t)((a0 + m) / d);

        /* store a in period */
        if (period_len == period_cap) {
            size_t new_cap = period_cap ? period_cap << 1 : 64;
            period_vec = (idx_t *)realloc(period_vec, new_cap * sizeof(idx_t));
            period_cap = new_cap;
        }
        period_vec[period_len++] = a;

        /* period ends when a == 2*a0 */
        if (a == 2 * a0)
            break;
    }

    /* period must have even length */
    if (period_len & 1)
        return false;

    /* central term after removing last term */
    size_t mid_index = period_len / 2 - 1;
    return period_vec[mid_index] == 35;
}

/* Cached sequence values (1‑based indexing) */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;     /* number of stored elements (incl. dummy 0) */
static idx_t next_k = 2;             /* next integer to test */

/* Return the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (seq_cache_len == 0) {
        seq_cache_cap = 64;
        seq_cache = seq_alloc(seq_cache_cap);
        seq_cache[0] = 0;          /* dummy for 1‑based index */
        seq_cache_len = 1;
        next_k = 2;
    }

    while (seq_cache_len - 1 < (size_t)n) {
        while (!satisfies(next_k))
            ++next_k;

        if (seq_cache_len == seq_cache_cap) {
            seq_cache_cap <<= 1;
            seq_cache = seq_realloc(seq_cache, seq_cache_len, seq_cache_cap);
        }
        seq_cache[seq_cache_len++] = (seq_t)next_k;
        ++next_k;
    }

    return seq_cache[n];
}
