/*
 * A042933  Denominators of continued fraction convergents to sqrt(998).
 * https://oeis.org/A042933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of the continued‑fraction convergents to sqrt(998).
   OEIS formula:
       a(n) = a(n-1)*a_cur + a(n-2)   (with a_cur being the nth partial quotient)
   Recurrence for the partial quotients (D = 998):
       m_{k+1} = d_k * a_k - m_k
       d_{k+1} = (D - m_{k+1}^2) / d_k
       a_{k+1} = floor((floor(sqrt(D)) + m_{k+1}) / d_{k+1})
   Memoisation of the sequence is used, so calls may arrive in any order. */

#include <stdlib.h>
#include <stdint.h>

/* Helper: grow a seq_t array to at least `needed` elements */
static void ensure_capacity(seq_t **p, size_t *cap, size_t needed)
{
    if (*cap < needed)
    {
        size_t newcap = (*cap == 0) ? 1 : *cap;
        while (newcap < needed)
            newcap <<= 1;                 /* double until enough */
        seq_t *tmp = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
        if (tmp) /* realloc may fail; ignore for the build's simplicity */ {
            *p = tmp;
            *cap = newcap;
        }
    }
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    /* static cache of denominators q_k (i.e. a(k)) */
    static seq_t *q = NULL;   /* array of denominators */
    static size_t len = 0;    /* number of terms stored (len>0 ⇔ q[0] exists) */
    static size_t cap = 0;    /* current capacity of q */

    /* state variables for continued‑fraction expansion of sqrt(998) */
    static unsigned long long m = 0;   /* m_k */
    static unsigned long long d = 1;   /* d_k */
    static unsigned long long a_cur = 1; /* a_k; starts with a1 after initialisation */

    const unsigned long long D  = 998;
    const unsigned long long a0 = 31;   /* floor(sqrt(D)) */

    /* initialise on the first call */
    if (len == 0)
    {
        /* q[0] = 1 (first denominator) */
        ensure_capacity(&q, &cap, 1);
        q[0] = (seq_t)1;
        len = 1;

        /* compute partial quotient a1 to start the recurrence */
        unsigned long long m1 = d * a0 - m;            /* m1 */
        unsigned long long d1 = (D - m1 * m1) / d;    /* d1 */
        unsigned long long a1 = (a0 + m1) / d1;       /* a1 */
        m = m1; d = d1; a_cur = a1;  /* a_cur now holds a1 */
    }

    /* extend cache up to index n */
    while ((size_t)len <= (size_t)n)
    {
        /* compute next denominator q_len */
        seq_t prev     = q[len - 1];
        seq_t prevprev = (len >= 2) ? q[len - 2] : (seq_t)0;

        seq_t term = ( (seq_t)a_cur ) * prev + prevprev; /* q_len = a_cur * q_{len-1} + q_{len-2} */

        /* store the term */
        ensure_capacity(&q, &cap, len + 1);
        q[len] = term;
        ++len;

        /* advance to next partial quotient a_{len+1} */
        unsigned long long m_next = d * a_cur - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_cur = a_next;
    }

    return q[n];
}
