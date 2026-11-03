/*
 * A041865  Denominators of continued fraction convergents to sqrt(454).
 * https://oeis.org/A041865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 33832080168351370*a(n-34) - a(n-68) for n>67. Base terms
 *            a(0)..a(67) are the denominators of the convergents to
 *            sqrt(454). They are generated from the periodic continued
 *            fraction of sqrt(454)
 * Range:     u8 none, u16 n <= 10, u32 n <= 22, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 164 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* coefficient of the linear recurrence */
static const seq_t COEF = (seq_t)33832080168351370ULL;

/* generate the first 68 denominators of the continued‑fraction of sqrt(454) */
static void generate_initial_terms(seq_t *arr)
{
    const idx_t D = 454;                         /* the discriminant */
    const idx_t sqrtD = (idx_t)floor_sqrt((seq_t)D); /* floor(sqrt(454)) = 21 */

    idx_t a = sqrtD;     /* integer part of the term */
    idx_t m = 0;
    idx_t d = 1;

    seq_t p_prev2 = 0, p_prev1 = 1; /* p[-2], p[-1] of the recurrence for convergents */
    seq_t q_prev2 = 1, q_prev1 = 0; /* q[-2], q[-1] of the recurrence for convergents */

    for (idx_t i = 0; i < 68; ++i) {
        /* compute the next convergent */
        seq_t p_cur = (seq_t)a * p_prev1 + p_prev2;
        seq_t q_cur = (seq_t)a * q_prev1 + q_prev2;

        arr[i] = q_cur;  /* store the denominator */

        /* shift the previous values for the next iteration */
        p_prev2 = p_prev1;  p_prev1 = p_cur;
        q_prev2 = q_prev1;  q_prev1 = q_cur;

        /* next periodic continued‑fraction term */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (sqrtD + m_next) / d_next;

        m = m_next;  d = d_next;  a = a_next;
    }
}

/* cache for memoised values */
static seq_t *cache      = NULL;
static idx_t  cached_len = 0;

/* required public function */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache = mem_new(seq_t, 68);
        if (cache == NULL) return (seq_t)0;     /* allocation failure */
        generate_initial_terms(cache);
        cached_len = 68;
    }

    /* extend cache if required */
    if (n >= cached_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (tmp == NULL) return (seq_t)0;       /* allocation failure */
        cache = tmp;
        for (idx_t i = cached_len; i <= n; ++i)
            cache[i] = COEF * cache[i - 34] - cache[i - 68];
        cached_len = new_len;
    }

    return cache[n];
}
