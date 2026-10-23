/*
 * A042432  Numerators of continued fraction convergents to sqrt(744).
 * https://oeis.org/A042432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent in the continued fraction
 *            expansion of sqrt(744). First terms: 27, 82, 109, …
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *num_cache = NULL;   /* cache of computed numerators */
static size_t num_cap   = 0;      /* allocated capacity of num_cache   */
static idx_t last_index = 0;     /* highest index already computed   */
static bool   initialized   = false;   /* initialization flag */

static const uint64_t D   = 744;   /* D in sqrt(D) */
static const uint64_t A0  = 27;    /* floor(sqrt(744)) */

/* Continued‑fraction iteration state for sqrt(D) */
static uint64_t mf = 0;   /* m_k   */
static uint64_t df = 1;   /* d_k   */
static uint64_t af = A0;  /* a_k   */

seq_t a(idx_t n)
{
    /* one‑time initialization */
    if (!initialized) {
        /* allocate minimal cache */
        num_cap   = 1;
        num_cache = (seq_t *)malloc(num_cap * sizeof(seq_t));

        /* first term a(0) = floor(sqrt(D)) */
        num_cache[0] = (seq_t)A0;
        last_index  = 0;

        /* set continued‑fraction state to the 0th index */
        mf = 0;
        df = 1;
        af = A0;

        initialized = true;
    }

    /* return cached value if already computed */
    if (n <= last_index)
        return num_cache[n];

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;
    if (num_cap < need) {
        size_t new_cap = num_cap;
        if (!new_cap) new_cap = 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(num_cache, new_cap * sizeof(seq_t));
        num_cache = tmp;
        num_cap   = new_cap;
    }

    /* previous two numerators for the recurrence */
    seq_t p_prev      = num_cache[last_index];
    seq_t p_prev_prev = (last_index >= 1) ? num_cache[last_index - 1] : (seq_t)1;

    /* iterate from the next index up to n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        /* compute next m, d, a for continued fraction of sqrt(D) */
        uint64_t m_next = df * af - mf;
        uint64_t d_next = (D - m_next * m_next) / df;
        uint64_t a_next = (A0 + m_next) / d_next;

        /* new numerator: a_next * p_prev + p_prev_prev */
        seq_t a_next_seq = (seq_t)a_next;
        seq_t new_p      = a_next_seq * p_prev + p_prev_prev;

        num_cache[i] = new_p;

        /* update previous two values */
        p_prev_prev = p_prev;
        p_prev      = new_p;

        /* update continued‑fraction state */
        mf = m_next;
        df = d_next;
        af = a_next;
    }

    last_index = n;
    return num_cache[n];
}
