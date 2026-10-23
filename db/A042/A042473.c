/*
 * A042473  Denominators of continued fraction convergents to sqrt(764).
 * https://oeis.org/A042473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = denominator of the nth convergent to sqrt(764).
 * The convergents are generated via the standard recurrence for a quadratic
 * irrational, using the continued–fraction coefficients of sqrt(764):
 *
 *   m_{k+1} = d_k * a_k – m_k
 *   d_{k+1} = (764 – m_{k+1}^2) / d_k
 *   a_{k+1} = ⌊(27 + m_{k+1}) / d_{k+1}⌋
 *
 *   p_k = a_k * p_{k-1} + p_{k-2}
 *   q_k = a_k * q_{k-1} + q_{k-2}
 *
 * with p_{-1}=1, p_0=27 and q_{-1}=0, q_0=1.  a(0)=q_0=1.
 * The function memoises the values in a static cache so any call
 * order is supported and each term is computed in O(1) time after
 * the preceding one has been stored.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq_array.h"

/* cached denominators */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* state of the continued fraction of sqrt(764) */
static idx_t m_cur = 0;      /* m_k */
static idx_t d_cur = 1;      /* d_k */
static idx_t a_cur = 27;     /* a_k */

/* previous two convergents */
static seq_t p_prevprev = 1; /* p_{k-2} */
static seq_t p_prev     = 27;/* p_{k-1} */
static seq_t q_prevprev = 0; /* q_{k-2} */
static seq_t q_prev     = 1; /* q_{k-1} */

/* ensure there is space for at least n+1 cached terms */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        /* grow the cache at least to the required size */
        grow_seq_array(&cache, &cache_cap, (size_t)(n + 1));
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise static data on first call */
    if (!cache) {
        grow_seq_array(&cache, &cache_cap, 1);   /* cache[0] */
        cache[0] = 1;                           /* denominator of 0th convergent */
        cache_max = 0;

        /* initial continued–fraction state corresponds to a0 */
        m_cur = 0;
        d_cur = 1;
        a_cur = 27;
    }

    /* if requested value is already cached return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* compute successive convergents until we reach n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* next continued–fraction coefficient */
        idx_t m_next = d_cur * a_cur - m_cur;
        idx_t d_next = (764 - m_next * m_next) / d_cur;
        idx_t a_next = (27 + m_next) / d_next;

        /* next convergent */
        seq_t p_k = (seq_t)a_next * p_prev + p_prevprev;
        seq_t q_k = (seq_t)a_next * q_prev + q_prevprev;

        /* store the denominator */
        ensure_cache(k);
        cache[k] = q_k;

        /* shift variables for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_k;
        q_prevprev = q_prev;
        q_prev     = q_k;

        m_cur = m_next;
        d_cur = d_next;
        a_cur = a_next;
    }

    cache_max = n;
    return cache[n];
}
