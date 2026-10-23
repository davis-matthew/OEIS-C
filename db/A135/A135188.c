/*
 * A135188  Numbers n that raised to the powers from 1 to k (with k>=1) are
 *          multiple of the sum of their digits (n raised to k+1 must not be a
 *          multiple). Case k=3.
 * https://oeis.org/A135188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 11, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that
 *   n, n², n³ are multiples of the sum of their digits,
 *   but n⁴ is NOT a multiple of the sum of its digits.
 * This is OEIS A135188 with k = 3.
 * The function computes the n-th term by incremental search,
 * memoizing found values for successive calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

idx_t _floor_isqrt_u64(idx_t);  /* unused; included for compile unit */
#include "../utils/digit_sum.h"   /* digit_sum(seq_t) */
#include "../utils/pow_uint.h"    /* pow_uint(uint64_t, idx_t) */

static seq_t *seq_cache = NULL;   /* memoized sequence */
static idx_t  cache_len = 0;      /* number of items stored */
static idx_t  cache_cap = 0;      /* allocated capacity */
static uint64_t next_candidate = 1;  /* next integer to test */

/* Helper: check whether a given integer satisfies the property */
static bool ok_candidate(uint64_t x)
{
    /* Convert to seq_t for big‑int operations */
    seq_t n   = (seq_t)x;
    seq_t n2  = pow_uint((uint64_t)x, 2);
    seq_t n3  = pow_uint((uint64_t)x, 3);
    seq_t n4  = pow_uint((uint64_t)x, 4);

    seq_t s_n  = digit_sum(n);
    seq_t s_n2 = digit_sum(n2);
    seq_t s_n3 = digit_sum(n3);
    seq_t s_n4 = digit_sum(n4);

    if (n  % s_n  != 0) return false;
    if (n2 % s_n2 != 0) return false;
    if (n3 % s_n3 != 0) return false;
    if (n4 % s_n4 == 0) return false;   /* n⁴ must NOT be a multiple */

    return true;
}

/* Main entry:
 *   a(n) returns the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Ensure the cache is long enough for requested index */
    while (cache_len < n)
    {
        /* Test successive candidates until we find another term */
        while (!ok_candidate(next_candidate))
            ++next_candidate;

        /* Store valid term */
        if (cache_len == cache_cap)
        {
            size_t newcap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
            seq_cache = tmp;
            cache_cap = newcap;
        }
        seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
