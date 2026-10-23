/*
 * A041348  Numerators of continued fraction convergents to sqrt(188).
 * https://oeis.org/A041348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent of sqrt(188)
 *            Recurrence: p_{-1}=1, p_0=13 m_0=0, d_0=1, a_0=13 for n≥1: m_n =
 *            d_{n-1} * a_{n-1} - m_{n-1} d_n = (188 - m_n*m_n) / d_{n-1} a_n
 *            = (13 + m_n) / d_n p_n = a_n * p_{n-1} + p_{n-2} (a_n are the
 *            continued‑fraction coefficients of sqrt(188).)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static state for memoisation */
static seq_t   *cache        = NULL;   /* array of computed numerators p_n  */
static idx_t    cache_size   = 0;      /* number of entries currently stored */
static idx_t    m            = 0;      /* current m in the sqrt(188) algorithm   */
static idx_t    d            = 1;      /* current d                               */
static idx_t    a_prev       = 13;     /* a_{n-1} : previous continued‑fraction coefficient */
static const idx_t  D      = 188;      /* radicand */
static const idx_t  a0     = 13;       /* floor(sqrt(188)) */

/* ------------------------------------------------------------------ */
/*  Function: a
 *  Input : n  – the index (0‑based)
 *  Output: numerator of the n‑th convergent to sqrt(188)
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --- initialise on first call --------------------------------- */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure – unlikely */
        cache[0] = a0;                       /* p_0 = a0 = 13 */
        cache_size = 1;
        m = 0; d = 1; a_prev = a0;
    }

    /* --- return cached value if already computed ----------------- */
    if (n < cache_size)
        return cache[n];

    /* --- grow cache to hold up to index n ------------------------ */
    idx_t    start = cache_size;                 /* first index to compute  */
    if ((idx_t)(n + 1) > cache_size) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failure – unlikely */
        cache_size = n + 1;
    }

    /* --- compute missing terms ----------------------------------- */
    for (idx_t k = start; k <= n; ++k) {
        /* compute next continued‑fraction data */
        idx_t  m_next = d * a_prev - m;
        idx_t  d_next = (D - m_next * m_next) / d;
        idx_t  a_k    = (a0 + m_next) / d_next;

        /* compute the convergent numerator */
        seq_t  p_k;
        if (k == 1) {
            /* p_1 = a_1 * p_0 + p_{-1}  (p_{-1} = 1) */
            p_k = (seq_t)a_k * cache[0] + (seq_t)1;
        } else {
            p_k = (seq_t)a_k * cache[k - 1] + cache[k - 2];
        }

        cache[k] = p_k;                 /* store result */

        /* update state for next iteration */
        m      = m_next;
        d      = d_next;
        a_prev = a_k;
    }

    return cache[n];
}
