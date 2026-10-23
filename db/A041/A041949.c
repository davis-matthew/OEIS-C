/*
 * A041949  Denominators of continued fraction convergents to sqrt(497).
 * https://oeis.org/A041949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041949:
   Denominators of continued‑fraction convergents to sqrt(497).
   The recurrence is  a(n) = 2403774*a(n-10) - a(n-20)  (n>19).
   Here we generate terms directly from the continued‑fraction
   representation of sqrt(497); the recurrence holds for all n>19
   but the algorithm is simpler and works for any n>=0. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Constants for the continued fraction of sqrt(497) */
static const seq_t D     = (seq_t)497;   /* D = 497 */
static const seq_t a0    = (seq_t)22;    /* a0 = floor(sqrt(497)) */

/* Cache for denominators q_k (a(k)) */
static seq_t *denom    = NULL;          /* array of denominators */
static size_t denom_cap = 0;            /* current allocated capacity */
static idx_t   denom_max = (idx_t)-1;   /* largest index computed so far */

/* Current state of the continued‑fraction algorithm */
static seq_t m = (seq_t)0;   /* m_k   */
static seq_t d = (seq_t)1;   /* d_k   */
static seq_t a_prev = a0;    /* a_k   */

/* Ensure the denominator array has room for at least 'needed' elements */
static void ensure_denom_cap(size_t needed)
{
    if (needed <= denom_cap) return;
    size_t new_cap = denom_cap ? denom_cap : 1;
    while (new_cap < needed) new_cap *= 2;
    denom = (seq_t *)realloc(denom, new_cap * sizeof(seq_t));
    denom_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the first term directly */
    if (n == 0) return (seq_t)1;

    /* Initialise cache and state on first call */
    if (denom == NULL) {
        /* Allocate space for q_0 and q_1 */
        ensure_denom_cap(2);
        denom[0] = (seq_t)1;                 /* q_0 = 1          */
        /* Compute a_1 from the algorithm */
        seq_t m_next  = d * a_prev - m;      /* m_1 = 1*a0 - m0  */
        seq_t d_next  = (D - m_next * m_next) / d;
        seq_t a_next  = (a0 + m_next) / d_next;
        denom[1] = a_next;                   /* q_1 = a_1         */
        /* Update state for subsequent steps (k = 2) */
        m = m_next;       d = d_next;        a_prev = a_next;
        denom_max = 1;
    }

    /* Generate terms until we reach the requested index */
    while (denom_max < n) {
        seq_t m_next = d * a_prev - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        ensure_denom_cap(denom_max + 2);
        denom[denom_max + 1] = a_next * denom[denom_max] + denom[denom_max - 1];

        /* Advance the state */
        m   = m_next;
        d   = d_next;
        a_prev = a_next;
        denom_max++;
    }

    return denom[n];
}
