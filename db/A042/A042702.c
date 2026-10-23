/*
 * A042702  Numerators of continued fraction convergents to sqrt(881).
 * https://oeis.org/A042702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(881).
   Recurrence:   p_0 = 29
                  p_1 = 30
                  p_n = a_n * p_{n-1} + p_{n-2},
   where a_n are the partial quotients of the continued fraction of sqrt(881) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed numerators */
    static seq_t *p = NULL;
    static size_t size = 0;   /* number of terms stored (>=1 after init) */
    static size_t cap = 0;    /* allocated capacity */

    /* recurrence state variables for continued fraction */
    static seq_t m_last = 0;   /* m_k */
    static seq_t d_last = 1;   /* d_k */
    static seq_t a_last = 29;  /* a_k */

    /* recurrence state for numerators */
    static seq_t prev1 = 0; /* p_{k-1} */
    static seq_t prev2 = 0; /* p_{k-2} */

    /* ensure initialization */
    if (size == 0)
    {
        /* first allocation */
        size_t init_cap = 1024;
        p = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cap = init_cap;

        /* initial term p_0 = 29 */
        p[0] = 29;
        size = 1;

        /* set states for next iteration (k = 0) */
        prev1 = 29;        /* p_0  */
        prev2 = 1;         /* p_{-1} for recurrence */
        m_last = 0;        /* m_0 */
        d_last = 1;        /* d_0 */
        a_last = 29;       /* a_0 */
    }

    /* compute until we have up to index n */
    while (size <= n)
    {
        /* compute next partial quotient a_{k+1} */
        seq_t m_next = d_last * a_last - m_last;
        seq_t d_next = (881 - m_next * m_next) / d_last;
        seq_t a_next = (29 + m_next) / d_next;   /* floor division */

        /* compute next numerator p_{k+1} */
        seq_t p_next = a_next * prev1 + prev2;

        /* grow cache if needed */
        if (size >= cap)
        {
            size_t new_cap = cap * 2;
            p = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* store and move state forward */
        p[size] = p_next;
        size++;

        /* update recurrence states for next step */
        prev2 = prev1;
        prev1 = p_next;
        m_last = m_next;
        d_last = d_next;
        a_last = a_next;
    }

    return p[n];
}
