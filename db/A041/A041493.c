/*
 * A041493  Denominators of continued fraction convergents to sqrt(263).
 * https://oeis.org/A041493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n-th convergent of sqrt(263)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(263) */
    static const idx_t D   = 263;   /* discriminant */
    static const idx_t A0  = 16;    /* floor(sqrt(D)) */

    /* Cached denominators */
    static seq_t *denoms   = NULL;
    static size_t  cap     = 0;
    static size_t  len     = 0;

    /* Continued fraction state */
    static idx_t  m      = 0;
    static idx_t  d      = 1;
    static idx_t  a_cur  = A0;  /* a_k for current k (starting k=0) */
    static idx_t  k      = 0;    /* index of current a_k */

    /* Initialize caches and CF state on first call */
    if (!denoms) {
        cap = 4;
        denoms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!denoms) abort();
        denoms[0] = (seq_t)1;   /* a(0) */
        len = 1;

        m = 0;
        d = 1;
        a_cur = A0;
        k = 0;
    }

    /* Return cached value if available */
    if (n < len)
        return denoms[n];

    /* Extend cache to include needed terms */
    for (idx_t i = len; i <= n; ++i) {
        /* Compute next partial quotient a_{k+1} */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (A0 + m_next) / d_next;

        /* Update continued fraction state */
        m = m_next;
        d = d_next;
        a_cur = a_next;
        ++k;

        /* Compute new denominator using recurrence */
        seq_t q;
        if (i == 1) {
            q = (seq_t)a_next;  /* a(1) */
        } else {
            q = (seq_t)a_next * denoms[i - 1] + denoms[i - 2];
        }

        /* Grow cache if necessary */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            denoms = tmp;
            cap = new_cap;
        }

        denoms[len++] = q;
    }

    return denoms[n];
}
