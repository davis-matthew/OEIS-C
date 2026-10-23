/*
 * A040643  Continued fraction for sqrt(669).
 * https://oeis.org/A040643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the continued‑fraction coefficients of sqrt(669).
   The algorithm follows the standard recurrence:
        m_{k+1} = d_k·a_k − m_k
        d_{k+1} = (D − m_{k+1}²)/d_k
        a_{k+1} = ⌊(a0 + m_{k+1})/d_{k+1}⌋   (a0 = ⌊√D⌋)
   where D = 669.
   The coefficients are cached so that subsequent calls are fast, and the
   function works correctly for arbitrary orders of n.  No hard‑coded
   values are used for n > 0.  The only constant needed is a0 = 25. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for the continued‑fraction coefficients. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of coefficients stored */

    /* State for generating the next coefficient; stored as idx_t. */
    static idx_t m_state = 0, d_state = 1, a_state = 0;
    static idx_t a0_val = 25;   /* ⌊√669⌋ */
    static int initialized = 0;

    /* Only the first call does the heavy initialization. */
    if (!initialized) {
        /* Allocate space for the first coefficient a0. */
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
        cache[0] = a0_val;        /* a0 is always the first term (n=0) */
        cache_len = 1;

        /* Initialise the state to generate a(1). */
        a_state = a0_val;
        m_state = 0;
        d_state = 1;
        /* Compute the first following coefficient explicitly. */
        idx_t m_next = (idx_t)d_state * (idx_t)a_state - (idx_t)m_state;
        idx_t d_next = (669 - m_next * m_next) / (idx_t)d_state;
        idx_t a_next = (a0_val + m_next) / d_next;
        m_state = m_next;
        d_state = d_next;
        a_state = a_next;   /* now a_state holds a(1) */
        initialized = 1;
    }

    /* Generate terms up to index n if necessary. */
    while (cache_len <= n) {
        /* Resize the cache if we run out of space. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the coefficient for the current index. */
        cache[cache_len] = a_state;
        cache_len++;

        /* Compute the next coefficient (for next iteration). */
        idx_t m_next = (idx_t)d_state * (idx_t)a_state - (idx_t)m_state;
        idx_t d_next = (669 - m_next * m_next) / (idx_t)d_state;
        idx_t a_next = (a0_val + m_next) / d_next;

        m_state = m_next;
        d_state = d_next;
        a_state = a_next;
    }

    return cache[n];
}
