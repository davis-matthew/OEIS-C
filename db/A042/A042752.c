/*
 * A042752  Numerators of continued fraction convergents to sqrt(907).
 * https://oeis.org/A042752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of nth convergent of sqrt(907)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed numerators */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* state variables for continued fraction of sqrt(907) */
    static const idx_t D     = 907;     /* the square‑free integer */
    static const idx_t a0    = 30;      /* floor(sqrt(907)) */
    static idx_t m = 0, d = 1;          /* previous values of the expansion */
    static idx_t a_cur = a0;            /* current partial quotient a_i */
    static seq_t p_prev1 = 0, p_prev2 = 1;  /* p_{i-1} and p_{i-2} */

    /* ensure array capacity */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute needed terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_t p0 = (seq_t)a0;       /* p_0 = a0 */
            cache[0] = p0;
            p_prev1 = p0;
            p_prev2 = 1;                /* p_{-1} = 1 */
            cache_len = 1;
        } else {
            idx_t m_next = d * a_cur - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            seq_t p_next = ((seq_t)a_next) * p_prev1 + p_prev2;
            cache[i] = p_next;

            /* shift state for next iteration */
            m = m_next;
            d = d_next;
            a_cur = a_next;

            p_prev2 = p_prev1;
            p_prev1 = p_next;

            cache_len = i + 1;
        }
    }

    return cache[n];
}
