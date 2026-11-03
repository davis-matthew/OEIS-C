/*
 * A048565  a(n+1) is next smallest nontrivial cube beginning with a(n),
 *          initial cube is 8.
 * https://oeis.org/A048565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 5 (2026-11-03)
 * Helpers:   ipow_i32, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Next smallest nontrivial cube beginning with a(n‑1).
   a(0) = 8.
   A nontrivial cube is one that is not simply a(n‑1) followed by zeros
   (i.e. not of the form a(n‑1)·10^k).  The algorithm searches
   progressive ranges of the form
        [cur·10^t , (cur+1)·10^t – 1]
   for the first cube m³ that lies in a range and satisfies m³ > cur·10^t. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_cap = 0;          /* allocated capacity */
static size_t cache_len = 0;          /* number of terms stored */

/* Helper: next nontrivial cube beginning with cur. */
static seq_t next_term(seq_t cur)
{
    /* first root strictly larger than cube root of cur  */
    seq_t root = floor_cube_root(cur) + (seq_t)1;

    seq_t pow10 = (seq_t)10;          /* 10^t, t starts at 1 */
    seq_t cur_plus = cur + (seq_t)1;  /* (cur+1) */

    for (;;) {
        seq_t lower = cur * pow10;                     /* cur·10^t   */
        seq_t upper = cur_plus * pow10 - (seq_t)1;     /* (cur+1)·10^t – 1 */

        seq_t m_start = root;
        seq_t m_end   = floor_cube_root(upper);        /* largest m with m³ ≤ upper */

        if (m_start <= m_end) {
            /* Binary search smallest m in [m_start, m_end] with m³ ≥ lower */
            seq_t lo = m_start, hi = m_end;
            while (lo < hi) {
                seq_t mid = (lo + hi) / (seq_t)2;
                if (ipow_idx(mid, 3) < lower)
                    lo = mid + (seq_t)1;
                else
                    hi = mid;
            }
            seq_t cand = ipow_idx(lo, 3);
            if (cand <= upper && cand > lower)          /* nontrivial cube */
                return cand;
        }

        /* no cube in this range – try next t */
        pow10 *= (seq_t)10;
    }
}

/* Main entry point */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) if needed */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)8;          /* a(0) = 8 */
        cache_len = 1;
    }

    /* Expand cache up to requested index */
    while (cache_len <= n) {
        seq_t cur = cache[cache_len - 1];
        seq_t nxt = next_term(cur);

        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = nxt;
    }

    return cache[n];
}
