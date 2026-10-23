/*
 * A041457  Denominators of continued fraction convergents to sqrt(244).
 * https://oeis.org/A041457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction of sqrt(244) Recurrence: a(0)=1, a(1)=1,
 *            a(n)=3532638098*a(n-26)-a(n-52) for n>51 Computed by standard
 *            convergent generation (see code)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *den = NULL;         /* cache of denominators */
static size_t den_cap = 0;        /* current allocated capacity in elements */
static idx_t den_len = 0;         /* number of terms computed so far */

/* Continued‑fraction state for sqrt(244) */
static uint64_t cf_m = 0;         /* current m  */
static uint64_t cf_d = 1;         /* current d  */
static uint64_t cf_a = 15;        /* current a  */

/* Previous convergent values */
static seq_t p_prevprev = 0;
static seq_t p_prev = 1;
static seq_t q_prevprev = 1;
static seq_t q_prev = 0;

/* Helper to grow the cache. */
static void grow_cache(void) {
    size_t new_cap = den_cap ? den_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    den = tmp;
    den_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise on first invocation */
    if (!den) {
        den_cap = 16;
        den = (seq_t *)malloc(den_cap * sizeof(seq_t));
        if (!den) abort();
        den_len = 0;

        cf_m = 0;
        cf_d = 1;
        cf_a = 15;

        p_prevprev = 0;
        p_prev = 1;
        q_prevprev = 1;
        q_prev = 0;
    }

    /* extend cache up to requested index n */
    while (den_len <= n) {
        /* compute next convergent */
        seq_t q_next = ((seq_t)cf_a) * q_prev + q_prevprev;
        seq_t p_next = ((seq_t)cf_a) * p_prev + p_prevprev;

        /* grow cache if necessary */
        if (den_len >= den_cap) grow_cache();

        den[den_len] = q_next;  /* store new denominator */

        /* update for next iteration */
        q_prevprev = q_prev;
        q_prev = q_next;
        p_prevprev = p_prev;
        p_prev = p_next;

        /* advance continued fraction terms for sqrt(244) */
        uint64_t m_next = cf_d * cf_a - cf_m;
        int64_t temp = (int64_t)244 - (int64_t)m_next * (int64_t)m_next;
        uint64_t d_next = (uint64_t)(temp / (int64_t)cf_d);
        uint64_t a_next = (15 + m_next) / d_next;

        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;

        den_len++;
    }

    return den[n];
}
