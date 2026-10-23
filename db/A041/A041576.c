/*
 * A041576  Numerators of continued fraction convergents to sqrt(306).
 * https://oeis.org/A041576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(306)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the n-th term of A041576. */
seq_t a(idx_t n)
{
    /* Constants for sqrt(306).  */
    const idx_t D   = 306;
    const idx_t a0  = 17;

    /* Static cache of numerators.  */
    static seq_t *p      = NULL;   /* array of computed numerators */
    static size_t cap    = 0;      /* current capacity of p */
    static idx_t  size   = 0;      /* number of elements stored in p */

    /* State variables for the continued‑fraction algorithm.  */
    static idx_t  m_state = 0;          /* m_k */
    static idx_t  d_state = 1;          /* d_k */
    static idx_t  a_state = a0;         /* a_k  (coefficient for the next term) */
    static seq_t  prevprev = 0;         /* p_{k-2}  */
    static seq_t  prev     = 0;         /* p_{k-1}  */

    /* Initialise on first call.  */
    if (p == NULL) {
        cap = 4;                     /* initial capacity */
        p = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!p) return 0;            /* allocation failed (unlikely) */
        prevprev = (seq_t)1;          /* p_{-1} = 1  */
        prev     = (seq_t)a0;         /* p_0   = a0  */
        p[0] = prev;                  /* store first term */
        size = 1;                     /* number of stored terms */

        /* Compute the first coefficient after a0, i.e. a1. */
        idx_t m_next = d_state * a_state - m_state;        /* 17 */
        idx_t d_next = (D - m_next * m_next) / d_state;     /* 17 */
        idx_t a_next = (a0 + m_next) / d_next;             /*   2 */
        m_state = m_next;
        d_state = d_next;
        a_state = a_next;
    }

    /* Return cached value if already computed.  */
    if (n < size) {
        return p[n];
    }

    /* Extend the cache up to index n.  */
    while (size <= n) {
        /* Compute next numerator:  a_k * p_{k-1} + p_{k-2}  */
        seq_t curr = (seq_t)a_state * prev + prevprev;

        /* Ensure capacity.  */
        if (size >= cap) {
            size_t newcap = cap << 1;
            seq_t *tmp = (seq_t *)realloc(p, newcap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            p = tmp;
            cap = newcap;
        }

        p[size] = curr;                /* store p_k */
        prevprev = prev;               /* shift for next term */
        prev = curr;

        /* Compute the next coefficient (a_{k+1}) for the next iteration. */
        idx_t m_next = d_state * a_state - m_state;
        idx_t d_next = (D - m_next * m_next) / d_state;
        idx_t a_next = (a0 + m_next) / d_next;

        m_state = m_next;
        d_state = d_next;
        a_state = a_next;

        ++size;
    }

    return p[n];
}
