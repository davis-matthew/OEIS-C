/*
 * A042267  Denominators of continued fraction convergents to sqrt(659).
 * https://oeis.org/A042267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: denominators of continued fraction convergents to sqrt(659).
   Recurrence:
     q_{-2} = 1,  q_{-1} = 0,
     q_n    = a_n * q_{n-1} + q_{n-2}
   where
     a_0 = floor(sqrt(659)) = 25,
     m_0 = 0, d_0 = 1,
     m_{k+1} = d_k * a_k - m_k,
     d_{k+1} = (659 - m_{k+1}^2) / d_k,
     a_{k+1} = floor( (a_0 + m_{k+1}) / d_{k+1} ).
   a(n) is q_n. */

#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* constant for sqrt(659) */
    const seq_t D  = (seq_t)659;
    const seq_t a0 = (seq_t)25;   /* floor(sqrt(659)) */

    /* static cache of denominators */
    static seq_t *den = NULL;    /* array of q_k values */
    static size_t  cap = 0;      /* allocated capacity */
    static size_t  len = 0;      /* number of entries stored */
    static idx_t   next_idx = 0; /* next index to compute */

    /* recurrence state */
    static seq_t m    = (seq_t)0; /* m_k */
    static seq_t d    = (seq_t)1; /* d_k */
    static seq_t a_cur= a0;       /* a_k */

    static seq_t prev_q1 = (seq_t)0; /* q_{k-1} */
    static seq_t prev_q2 = (seq_t)1; /* q_{k-2} */

    /* first call: initialise */
    if (!den) {
        /* allocate initial capacity */
        cap = 16;
        den = seq_alloc(cap);
        den[0] = (seq_t)1;          /* q_0 = 1 */
        len = 1;
        /* set recurrence history for the next step (k = 0 finished) */
        prev_q1 = den[0];           /* q_0 */
        prev_q2 = (seq_t)0;         /* q_{-1} */
        next_idx = 1;               /* next index to compute */
    }

    /* return cached value if available */
    if (n < len) {
        return den[n];
    }

    /* compute missing terms up to index n */
    for (idx_t i = next_idx; i <= n; ++i) {
        /* next partial quotient */
        m     = d * a_cur - m;
        d     = (D - m * m) / d;
        a_cur = (a0 + m) / d;

        /* k-th denominator */
        seq_t q = a_cur * prev_q1 + prev_q2;

        /* ensure sufficient capacity */
        if ((size_t)i >= cap) {
            size_t newcap = cap ? cap * 2 : 16;
            while (newcap <= (size_t)i) newcap <<= 1;
            den = seq_realloc(den, cap, newcap);
            cap = newcap;
        }

        den[i] = q;

        /* shift for next iteration */
        prev_q2 = prev_q1;
        prev_q1 = q;
    }

    len      = n + 1;
    next_idx = n + 1;
    return den[n];
}
