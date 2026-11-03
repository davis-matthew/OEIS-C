/*
 * A042929  Denominators of continued fraction convergents to sqrt(996).
 * https://oeis.org/A042929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 17107630*a(n-20) - a(n-40) for n>39. The first 40 terms
 *            are the denominators of the continued‑fraction convergents to
 *            sqrt(996)
 * Range:     u8 none, u16 n <= 16, u32 n <= 29, u64 n <= 56, u128 n <= 109,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 198 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* initialise first 40 terms via the continued‑fraction algorithm */
    if (cache == NULL) {
        const size_t init_cap = 40;
        cache = mem_new(seq_t, init_cap);
        if (cache == NULL) return 0;                     /* allocation failed */

        seq_t a0 = floor_sqrt((seq_t)996);               /* floor(sqrt(996)) = 31 */
        seq_t m   = 0;
        seq_t d   = 1;
        seq_t a_term = a0;

        seq_t p_prev2 = 0, p_prev1 = 1;                  /* p_{-2} = 0 , p_{-1} = 1 */
        seq_t q_prev2 = 1, q_prev1 = 0;                  /* q_{-2} = 1 , q_{-1} = 0 */

        for (idx_t i = 0; i < 40; ++i) {
            seq_t p = a_term * p_prev1 + p_prev2;
            seq_t q = a_term * q_prev1 + q_prev2;
            cache[i] = q;                                /* store denominator */

            if (i == 39) break;

            seq_t m_next = d * a_term - m;
            seq_t d_next = ((seq_t)996 - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a_term = a_next;

            /* shift previous convergents */
            p_prev2 = p_prev1;
            p_prev1 = p;
            q_prev2 = q_prev1;
            q_prev1 = q;
        }

        cap = init_cap;
        max_computed = 39;
    }

    /* return pre‑computed value if it exists */
    if (n < cap)
        return cache[n];

    /* extend cache to accommodate n */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap) {
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) return 0;                          /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms using the recurrence relation */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 39) continue;   /* already available */
        cache[i] = (17107630 * cache[i - 20]) - cache[i - 40];
    }
    max_computed = n;
    return cache[n];
}
