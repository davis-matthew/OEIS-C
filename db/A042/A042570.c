/*
 * A042570  Numerators of continued fraction convergents to sqrt(814).
 * https://oeis.org/A042570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = numerator of the n‑th convergent of the continued fraction for √814.
 * Recurrence: p_{-2}=0, p_{-1}=1, a0=floor(√814)=28.
 * For i ≥ 0:
 *   p_i   = a_i * p_{i‑1} + p_{i‑2}
 *   m_{i+1} = d_i * a_i - m_i
 *   d_{i+1} = (D - m_{i+1}^2) / d_i
 *   a_{i+1} = floor((a0 + m_{i+1}) / d_{i+1})
 * where D = 814.
 */
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* terms already computed (0..cache_size-1) */
    static size_t cache_cap  = 0;   /* allocated capacity in cache */

    /* continued fraction parameters for √814 */
    static const idx_t D = 814;      /* radicand */
    static idx_t a0;        /* floor(√814) = 28 */
    static idx_t m;         /* current m_i */
    static idx_t d;         /* current d_i */
    static idx_t a_i;       /* current a_i */
    static seq_t pm2;       /* p_{i‑2} */
    static seq_t pm1;       /* p_{i‑1} */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failure */

        a0 = _floor_isqrt_u64(D);
        pm2 = (seq_t)0;      /* p_{-2} */
        pm1 = (seq_t)1;      /* p_{-1} */
        m   = (idx_t)0;      /* m0   */
        d   = (idx_t)1;      /* d0   */
        a_i = a0;            /* a0   */
        cache_size = 0;
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 128;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to index n */
    while (cache_size <= (size_t)n) {
        /* current convergent numerator: p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t pi = ((seq_t)a_i) * pm1 + pm2;
        cache[cache_size] = pi;

        /* shift state for next iteration */
        pm2 = pm1;
        pm1 = pi;

        /* prepare next a_{i+1} if we need more terms */
        idx_t m_next = d * a_i - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_i = a_next;

        cache_size++;
    }

    return cache[(size_t)n];
}
