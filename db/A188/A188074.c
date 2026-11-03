/*
 * A188074  Positions of 1 in the zero-one sequence [n*r]-[3*r]-[n*r-3*r],
 *          where r=sqrt(3), n>=1.
 * https://oeis.org/A188074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k≥1 for which floor(k*sqrt(3)) -
 *            floor((k-3)*sqrt(3)) = 6
 * Range:     u8 none, u16 n <= 13, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This is the same as the positions of 1 in the zero‑one sequence
   [n*r] - [3*r] - [n*r-3*r] with r = sqrt(3).) */

#include <stdlib.h>
#include <stdint.h>
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

static seq_t *a_cache  = NULL;   /* stored positions */
static size_t a_len    = 0;       /* number of elements stored */
static size_t a_cap    = 0;       /* allocated capacity */
static seq_t   a_next_k = 1;      /* next k to examine */

/* resize cache if needed */
static void a_ensure_cap(size_t need) {
    if (a_cap >= need) return;
    size_t new_cap = a_cap ? a_cap << 1U : 16U;
    while (new_cap < need) new_cap <<= 1U;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) abort();              /* allocation failed */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* sequence a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* defined only for n≥1 */

    while (a_len < n) {
        seq_t k = a_next_k++;      /* next candidate */
        seq_t t1 = floor_sqrt((seq_t)3 * k * k);   /* floor(k*sqrt3) */

        int64_t diff;
        if (k >= 3) {
            seq_t km3 = k - 3U;
            seq_t t2  = floor_sqrt((seq_t)3 * km3 * km3); /* floor((k-3)*sqrt3) */
            diff = (int64_t)t1 - (int64_t)t2;
        } else {
            /* (k-3) is negative.  For integer s, floor(-s) = -(ceil(s)).
               ceil((3-k)*sqrt3) = floor((3-k)*sqrt3)+1 because sqrt3 is
               irrational.  Thus floor((k-3)*sqrt3) = -(floor((3-k)*sqrt3)+1). */
            seq_t m  = 3U - k;
            seq_t tm = floor_sqrt((seq_t)3 * m * m);      /* floor(m*sqrt3) */
            diff = (int64_t)t1 + (int64_t)(tm + 1U);
        }

        if (diff == 6) {
            a_ensure_cap(a_len + 1U);
            a_cache[a_len++] = k;
        }
    }
    return a_cache[n - 1U];
}
