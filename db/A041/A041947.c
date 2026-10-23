/*
 * A041947  Denominators of continued fraction convergents to sqrt(496).
 * https://oeis.org/A041947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 23, u64 n <= 47, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=3, a(2)=4, a(3)=11, a(4)=48, a(5)=59, a(6)=107, a(7)=273,
   a(8)=653, a(9)=1579, a(10)=2232, a(11)=3811, a(12)=17476, a(13)=38763,
   a(14)=56239, a(15)=207480,
   a(n)=9241598*a(n-16)-a(n-32) for n>31.
   Denominators of convergents to sqrt(496).
*/

#include <stdlib.h>
#include <stdbool.h>

/* Compute the n-th denominator of the continued fraction convergents to sqrt(496).  */
seq_t a(idx_t n)
{
    /* Static cache and recurrence state */
    static seq_t *den = NULL;          /* cache of computed denominators */
    static size_t  size = 0;           /* number of computed terms */
    static size_t  capacity = 0;       /* allocated capacity of den[] */
    static bool    initialized = false;

    /* State variables for generating the continued fraction */
    static seq_t prev_pp = 0;  /* p_{-2} */
    static seq_t prev_p  = 1;  /* p_{-1} */
    static seq_t prev_qq = 1;  /* q_{-2} */
    static seq_t prev_q  = 0;  /* q_{-1} */

    static seq_t m_cur = 0;    /* m for current step */
    static seq_t d_cur = 1;    /* d for current step */
    static seq_t a_cur = 22;   /* a_{current} */

    /* Constants */
    /* (seq_t) is sufficiently large for the required arithmetic */
    static const seq_t D   = 496;   /* the discriminant */
    static const seq_t A0  = 22;    /* floor(sqrt(496)) */

    /* Initialise state on first call */
    if (!initialized) {
        initialized = true;
        capacity = 0;
        size = 0;
    }

    /* Allocate or extend cache if necessary */
    if (den == NULL) {
        capacity = (size_t)(n + 1);
        if (capacity < 16) capacity = 16;   /* initial chunk */
        den = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Generate terms until the requested index is available */
    while (size <= n) {
        if (size >= capacity) {
            capacity *= 2;
            if (capacity == 0) capacity = 16;
            den = (seq_t *)realloc(den, capacity * sizeof(seq_t));
        }

        /* Compute next convergent numerator/denominator */
        seq_t p = a_cur * prev_p + prev_pp;
        seq_t q = a_cur * prev_q + prev_qq;
        den[size] = q;

        /* Shift the recurrence state for the next iteration */
        prev_pp = prev_p;
        prev_p  = p;
        prev_qq = prev_q;
        prev_q  = q;

        /* Advance continued‑fraction parameters to get a_{next} */
        seq_t m_next = d_cur * a_cur - m_cur;
        seq_t d_next = (D - m_next * m_next) / d_cur;
        seq_t a_next = (A0 + m_next) / d_next;   /* floor division */

        m_cur = m_next;
        d_cur = d_next;
        a_cur = a_next;

        size++;
    }

    return den[n];
}
