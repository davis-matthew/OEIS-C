/*
 * A042516  Numerators of continued fraction convergents to sqrt(787).
 * https://oeis.org/A042516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction convergent numerators for sqrt(787) (OEIS A042516). */
/* The numerator a(n) is computed by the standard recurrence:
 *   a(0) = floor(sqrt(787)) = 28
 *   p_{-2}=0, p_{-1}=1
 *   p_k   = a_k * p_{k-1} + p_{k-2}
 *   where a_k are the continued‑fraction coefficients of sqrt(787).  */
/* The function is memoised; subsequent calls in any order run in O(1). */

#include <stdlib.h>

static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed < *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap <= needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

static seq_t *p_arr = NULL;      /* numerators of convergents */
static size_t p_cap = 0;
static idx_t max_computed = (idx_t)-1;  /* largest n already computed */

static seq_t last_m = 0;    /* m_k of continued‑fraction */
static seq_t last_d = 1;    /* d_k of continued‑fraction */
static seq_t prev_a = (seq_t)28;  /* a_{k-1} */
static const seq_t A0 = (seq_t)28;   /* a0 = floor(sqrt(787)) */
static const seq_t S  = (seq_t)787;   /* value under the square root */

seq_t a(idx_t n)
{
    /* initialise on first use */
    if (max_computed == (idx_t)-1)
    {
        ensure_capacity(1, &p_cap, &p_arr);
        p_arr[0] = (seq_t)28;   /* a0 numerator */
        max_computed = 0;
    }

    /* cache hit */
    if (n <= max_computed) return p_arr[n];

    /* compute numerators up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k)
    {
        /* next continued‑fraction data */
        seq_t m_next = last_d * prev_a - last_m;
        seq_t d_next = (S - m_next * m_next) / last_d;
        seq_t a_next = (A0 + m_next) / d_next;  /* floor division */

        /* recurrence for numerator of convergents */
        seq_t p_prev = p_arr[k - 1];
        seq_t p_prev_prev = (k == 1) ? (seq_t)1 : p_arr[k - 2];
        seq_t p_new = a_next * p_prev + p_prev_prev;

        /* store result */
        ensure_capacity(k + 1, &p_cap, &p_arr);
        p_arr[k] = p_new;

        /* update continued‑fraction state for next iteration */
        last_m  = m_next;
        last_d  = d_next;
        prev_a  = a_next;
    }

    max_computed = n;
    return p_arr[n];
}
