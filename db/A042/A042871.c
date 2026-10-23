/*
 * A042871  Denominators of continued fraction convergents to sqrt(967).
 * https://oeis.org/A042871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of the continued
 *            fraction of sqrt(967)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static state used to memoize the sequence */
static seq_t *cache      = NULL;   /* array of denominators */
static size_t cache_size = 0;      /* allocated capacity */
static size_t cache_cnt  = 0;      /* number of computed terms */

/* continued fraction generation state */
static idx_t  m_val   = 0;         /* current m */
static idx_t  d_val   = 1;         /* current d */
static seq_t  q_prev2 = 1;         /* q_{-2} */
static seq_t  q_prev1 = 0;         /* q_{-1} */

/* constant a0 = floor(sqrt(967)) */
static const idx_t a0 = 31;

static void ensure_capacity(size_t needed)
{
    if (cache_size >= needed) return;
    size_t new_cap = cache_size == 0 ? 16 : cache_size * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failed */
    cache = tmp;
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    /* initialize on first call */
    if (!cache) {
        ensure_capacity(16);
        /* q0 = 1 */
        cache[0] = (seq_t)1;
        cache_cnt = 1;

        /* update continued‑fraction state after processing k = 0 */
        idx_t a_val = a0;                         /* 31 */
        idx_t m_next = d_val * a_val - m_val;     /* 31 */
        idx_t d_next = (967 - m_next * m_next) / d_val; /* 6 */
        m_val = m_next;
        d_val = d_next;

        /* shift q_prev values for the next iteration */
        seq_t old_q_prev1 = q_prev1;              /* 0 */
        seq_t old_q_prev2 = q_prev2;              /* 1 */
        q_prev2 = old_q_prev1;                    /* 0 */
        q_prev1 = cache[0];                       /* 1 */
    }

    /* generate terms up to index n */
    while (cache_cnt <= n) {
        /* next partial quotient a_k */
        idx_t a_val = (a0 + m_val) / d_val;

        /* denominator q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t qk = (seq_t) a_val * q_prev1 + q_prev2;

        /* store it */
        ensure_capacity(cache_cnt + 1);
        cache[cache_cnt++] = qk;

        /* update state for next k */
        idx_t m_next = d_val * a_val - m_val;
        idx_t d_next = (967 - m_next * m_next) / d_val;
        m_val = m_next;
        d_val = d_next;

        /* shift previous q's */
        q_prev2 = q_prev1;
        q_prev1 = qk;
    }

    return cache[n];
}
