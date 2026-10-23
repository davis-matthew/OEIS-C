/*
 * A042904  Numerators of continued fraction convergents to sqrt(984).
 * https://oeis.org/A042904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent of sqrt(984)'s
 *            continued fraction
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t D   = 984;   /* discriminant */
static const idx_t A0  = 31;    /* floor(sqrt(984)) */

static seq_t *cache        = NULL;   /* dynamic array of a(n) */
static size_t cache_cnt    = 0;      /* number of stored terms (cache size) */

/* State variables for the continued fraction expansion (last computed step) */
static idx_t m_state = 0;
static idx_t d_state = 1;
static idx_t a_state = A0;

/* Numerator recurrence: p_{-1}=1, p_0=A0, p_{k+1}=a_{k+1}*p_k+p_{k-1} */
static seq_t p_prev = 1;   /* p_{k-1} for next computation */
static seq_t p_curr = 0;   /* p_k for next computation (set on first init) */

static void ensure_cache(idx_t n)
{
    /* already have terms up to n if cache_cnt > n (since indices start at 0) */
    if (cache_cnt > n)
        return;

    /* extend the cache to hold indices 0..n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory (should not happen in tests) */
    cache = tmp;

    /* first call: initialise first element and recurrence state */
    if (cache_cnt == 0) {
        cache[0] = A0;                /* a(0) */
        p_curr   = A0;
        p_prev   = 1;
        m_state  = 0;
        d_state  = 1;
        a_state  = A0;
        cache_cnt = 1;
    }

    /* generate missing terms up to n */
    for (size_t i = cache_cnt; i <= n; ++i) {
        idx_t m_next = d_state * a_state - m_state;
        idx_t d_next = (D - m_next * m_next) / d_state;
        idx_t a_next = (A0 + m_next) / d_next;

        seq_t p_next = a_next * p_curr + p_prev;
        cache[i] = p_next;

        /* shift for next iteration */
        p_prev = p_curr;
        p_curr = p_next;
        m_state = m_next;
        d_state = d_next;
        a_state = a_next;
    }

    cache_cnt = new_size;           /* update cached size */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
