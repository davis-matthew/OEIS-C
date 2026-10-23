/*
 * A040678  Continued fraction for sqrt(705).
 * https://oeis.org/A040678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   int_sqrt705
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(705).  Uses the standard
   recurrence:
   m_k   = d_{k-1} * a_{k-1} - m_{k-1}
   d_k   = (705 - m_k^2) / d_{k-1}
   a_k   = floor((26 + m_k) / d_k)
   a(0)  = 26.  Caches computed terms for fast random access. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/int_sqrt705.h"

static struct {
    seq_t *cache;   /* cached terms */
    size_t size;    /* number of cached terms */
    size_t cap;     /* allocated capacity */
    seq_t a0;       /* floor(sqrt(705)) = 26 */
    seq_t m_prev;   /* m_{k-1} */
    seq_t d_prev;   /* d_{k-1} */
    seq_t a_prev;   /* a_{k-1} */
    idx_t next;     /* next index to compute */
} state = {NULL,0,0,0,0,0,0,0};

static void ensure_capacity(void)
{
    if (state.size + 1 > state.cap) {
        size_t new_cap = state.cap ? state.cap * 2 : 1;
        seq_t *tmp = (seq_t *)realloc(state.cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        state.cache = tmp;
        state.cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise state on first call */
    if (!state.cache) {
        state.cache = (seq_t *)malloc(sizeof(seq_t));
        if (!state.cache) exit(EXIT_FAILURE);
        state.cap = 1;
        state.a0 = int_sqrt705();          /* 26 for sqrt(705) */
        state.cache[0] = state.a0;         /* a(0) */
        state.size = 1;
        state.m_prev = 0;
        state.d_prev = 1;
        state.a_prev = state.a0;
        state.next = 1;
    }

    /* Compute terms up to n if necessary */
    while (state.next <= n) {
        seq_t m_next = state.d_prev * state.a_prev - state.m_prev;
        seq_t d_next = (705 - m_next * m_next) / state.d_prev;
        seq_t a_next = (state.a0 + m_next) / d_next;

        ensure_capacity();
        state.cache[state.size] = a_next;
        state.size++;

        /* Advance state */
        state.m_prev = m_next;
        state.d_prev = d_next;
        state.a_prev = a_next;
        state.next++;
    }

    return state.cache[n];
}
