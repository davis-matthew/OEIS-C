/*
 * A234335  Numbers k such that distances from k to three nearest squares are
 *          three perfect squares.
 * https://oeis.org/A234335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Enumerate natural numbers k such that the distances to the three
 *            nearest squares are all perfect squares
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   ipow_i32, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note:  this function searches integers sequentially,
    stores the valid k values in a static dynamic array, and returns the
    n-th element (1‑based).  The search continues until the requested
    index is satisfied.  The algorithm is O(n) and uses only integer
    arithmetic. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/isqrt.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 0;   /* next integer to test */

static bool is_square_seq(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for 0‑based callers */

    while (cache_len < n)
    {
        idx_t k = next_k;
        idx_t m = isqrt(k);      /* m^2 ≤ k < (m+1)^2 */

        seq_t m_sq   = (seq_t)m * (seq_t)m;
        seq_t next_sq = (seq_t)(m + 1) * (seq_t)(m + 1);
        seq_t d1 = (seq_t)k - m_sq;          /* distance to m^2   */
        seq_t d2 = next_sq - (seq_t)k;       /* distance to (m+1)^2 */

        seq_t d3;
        if (m > 0)
        {
            seq_t prev_sq   = (seq_t)(m - 1) * (seq_t)(m - 1);
            seq_t d_prev    = (seq_t)k - prev_sq;
            seq_t nextnext_sq = (seq_t)(m + 2) * (seq_t)(m + 2);
            seq_t d_nextnext = nextnext_sq - (seq_t)k;
            d3 = (d_prev < d_nextnext) ? d_prev : d_nextnext;
        }
        else
        {
            /* m == 0, so the third nearest square is (m+2)^2 */
            seq_t nextnext_sq = (seq_t)(m + 2) * (seq_t)(m + 2);
            d3 = nextnext_sq - (seq_t)k;
        }

        if (is_square_seq(d1) && is_square_seq(d2) && is_square_seq(d3))
        {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)k);
        }

        next_k = k + 1;
    }

    return cache[n - 1];
}
