/*
 * A042282  Numerators of continued fraction convergents to sqrt(667).
 * https://oeis.org/A042282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(667) using the continued‑fraction recurrence.
   Recurrence: p_{-2}=0, p_{-1}=1, p_0=25.
   For k≥1, let a_k be the k‑th partial quotient of sqrt(667);
   then p_k = a_k * p_{k-1} + p_{k-2}. */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0..k] = a(0), … , a(k) */
static idx_t cache_cap = 0;     /* number of elements currently cached */

/* Return the n-th numerator of the continued fraction convergents of sqrt(667). */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < cache_cap)
        return cache[n];

    /* Need to compute up to n, reallocating the cache if necessary. */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {               /* allocation failed – abort silently */
        return (seq_t)0;
    }
    cache = tmp;
    cache_cap = new_cap;

    /* Constants for sqrt(667). */
    const idx_t D   = 667;   /* radicand */
    const idx_t a0  = 25;    /* floor(sqrt(667)) */

    /* Base case: the 0th convergent. */
    cache[0] = (seq_t)a0;
    if (n == 0)
        return cache[0];

    /* Compute the first partial quotient a_1 and the 1st convergent. */
    idx_t m  = 0;          /* m_0 */
    idx_t d  = 1;          /* d_0 */
    idx_t m1 = d * a0 - m;            /* m_1 */
    idx_t d1 = (D - m1 * m1) / d;      /* d_1 */
    idx_t a1 = (a0 + m1) / d1;         /* a_1 */

    seq_t p_m2 = 0;          /* p_{-2} */
    seq_t p_m1 = 1;          /* p_{-1} */
    seq_t p0   = cache[0];   /* p_0    */
    seq_t p1   = (seq_t)a1 * p0 + p_m1; /* p_1    */

    cache[1] = p1;
    if (n == 1)
        return cache[1];

    /* Prepare for the loop. */
    idx_t prev_a = a1;  /* a_{k-1} */
    m = m1; d = d1;
    seq_t p_km2 = p0;    /* p_{k-2} for next iteration */
    seq_t p_km1 = p1;    /* p_{k-1} for next iteration */

    /* Compute convergents up to index n. */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t m_next = d * prev_a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        seq_t p_k = (seq_t)a_next * p_km1 + p_km2;
        cache[k] = p_k;

        /* Shift for next iteration. */
        p_km2 = p_km1;
        p_km1 = p_k;
        m = m_next;
        d = d_next;
        prev_a = a_next;
    }

    return cache[n];
}
