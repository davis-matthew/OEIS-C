/*
 * A115162  Positive numbers that are not the sum of a triangular number, a
 *          square and a cube, all of them greater than or equal to 1.
 * https://oeis.org/A115162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that cannot be expressed as T +
 *            S + C where T = t(t+1)/2, S = s^2, C = c^3, t,s,c ≥ 1
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

static bool is_representable(seq_t m)
{
    /* Minimum sum is 1 + 1 + 1 = 3, so 1 and 2 are never representable */
    if (m < (seq_t)3) return false;

    /* Iterate over possible cubes */
    for (idx_t c = 1; ; ++c)
    {
        seq_t c3 = (seq_t)c * (seq_t)c * (seq_t)c;
        if (c3 > m - (seq_t)2) break;       /* remaining must be ≥ 1 + 1 */
        seq_t rem1 = m - c3;

        /* Iterate over possible squares */
        for (idx_t s = 1; ; ++s)
        {
            seq_t s2 = (seq_t)s * (seq_t)s;
            if (s2 > rem1 - (seq_t)1) break; /* remaining must be ≥ 1 */
            seq_t rem2 = rem1 - s2;

            /* Is rem2 triangular?  t(t+1)/2 = rem2  */
            seq_t d   = 1 + rem2 * (seq_t)8;          /* discriminant */
            seq_t rt  = floor_sqrt(d);
            if (rt * rt == d && ((rt - (seq_t)1) % (seq_t)2 == 0))
                return true;                          /* representation found */
        }
    }
    return false;   /* no representation exists */
}

seq_t a(idx_t n)
{
    /* Memoization of computed terms */
    static seq_t *terms   = NULL;
    static size_t count   = 0;      /* number of stored terms */
    static size_t capacity = 0;      /* allocated capacity */
    static idx_t last_checked = 0;   /* largest integer examined so far */

    if (n == 0) return (seq_t)0;     /* undefined for n = 0 */

    /* Return cached value if already computed */
    if (n <= count)
        return terms[n - 1];

    /* Search for further non‑representable numbers */
    for (idx_t val = last_checked + 1; ; ++val)
    {
        if (!is_representable((seq_t)val))
        {
            /* Append to cache */
            if (count == capacity)
            {
                size_t newcap = capacity ? capacity * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, terms, newcap);
                if (!tmp) { mem_free(terms); return (seq_t)0; } /* allocation failure */
                terms = tmp;
                capacity = newcap;
            }
            terms[count++] = (seq_t)val;
            if (count >= n)
            {
                last_checked = val;
                return terms[n - 1];
            }
        }
        last_checked = val;
    }
}
