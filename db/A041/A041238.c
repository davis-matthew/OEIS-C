/*
 * A041238  Numerators of continued fraction convergents to sqrt(131).
 * https://oeis.org/A041238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: terms are numerators of convergents to sqrt(131).
   a(0) = 11, a(1) = 23. Their partial quotients are
   11,2,4,11,4,2,22,2,4,11,4,2,22,… and satisfy
   p[n] = a[n] * p[n-1] + p[n-2]  (p[−1] = 1, p[0] = 11).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed numerators */
    static seq_t *p = NULL;
    static idx_t len = 0;
    /* state of the continued‑fraction expansion */
    static idx_t m = 0, d = 1;
    static idx_t last_a = 0;   /* partial quotient a_{k‑1} */
    const idx_t D = 131;
    const idx_t a0 = 11;       /* floor(sqrt(D)) */

    /* initialise on first call */
    if (!p) {
        p = (seq_t *)malloc(sizeof(seq_t));
        if (!p) return (seq_t)0;     /* out of memory, unlikely */
        p[0] = a0;                   /* a(0) = 11 */
        len = 1;
        m = 0;
        d = 1;
        last_a = a0;                 /* a0 will be used to compute a1 */
    }

    if (n < len)
        return p[n];

    /* extend the cache up to index n */
    for (idx_t i = len; i <= n; ++i) {
        /* compute next partial quotient a_i */
        idx_t m_next = d * last_a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* update continued‑fraction state for the next step */
        m = m_next;
        d = d_next;
        last_a = a_next;

        seq_t val;
        if (i == 1)            /* p[1] = a1 * p[0] + 1 */
            val = (seq_t)a_next * p[0] + (seq_t)1;
        else                    /* p[i] = a_i * p[i-1] + p[i-2] */
            val = (seq_t)a_next * p[i-1] + p[i-2];

        seq_t *tmp = (seq_t *)realloc(p, (i + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory */
        p = tmp;
        p[i] = val;
        len = i + 1;
    }

    return p[n];
}
