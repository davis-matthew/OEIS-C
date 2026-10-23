/*
 * A041754  Numerators of continued fraction convergents to sqrt(397).
 * https://oeis.org/A041754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 40956605964*a(n-21) + a(n-42)   (n > 41)
   Initial terms (offset 0) are produced by the continued‑fraction
   expansion of √397: a(0)=19, a(1)=20, a(2)=259, a(3)=797, … */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Constants for √397 */
    static const unsigned long long D   = 397ULL;   /* D = 397 */
    static const unsigned long long A0  = 19ULL;    /* ⌊√397⌋ */

    /* continued–fraction state (kept between calls) */
    static unsigned long long m   = 0ULL; /* m_k   */
    static unsigned long long d   = 1ULL; /* d_k   */
    static unsigned long long a_coef = A0;/* a_k   (current coefficient) */

    /* Cached numerators of the convergents */
    static seq_t *P   = NULL; /* dynamic array of numerators */
    static size_t len = 0;    /* number of stored terms (next to compute) */
    static size_t cap = 0;    /* capacity of P                           */
    static int initialized = 0;

    /* One‑time array allocation and first term initialization */
    if (!initialized) {
        cap = 64;
        P = (seq_t *)realloc(P, cap * sizeof(seq_t));
        if (!P) return 0;                     /* out of memory */
        P[0] = (seq_t)A0;                     /* p_0 = a0 = 19 */
        len      = 1;                         /* next to compute is p_1 */
        initialized = 1;
    }

    /* Generate terms until we have a(n) */
    while (len <= (size_t)n) {
        /* Ensure the array is large enough for the next element */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                 /* out of memory */
            P = tmp;
            cap = new_cap;
        }

        /* Next continued‑fraction coefficient a_{k+1} */
        unsigned long long m_next = d * a_coef - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (A0 + m_next) / d_next;

        /* Convergent numerator recurrence: p_{k+1} = a_{k+1}*p_k + p_{k-1} */
        seq_t p_prev    = P[len - 1];               /* p_k   */
        seq_t p_prevprev = (len == 1) ? (seq_t)1 : P[len - 2]; /* p_{k-1} */
        seq_t a_next_seq = a_next;                 /* for seq_t arithmetic */

        seq_t p_next = a_next_seq * p_prev + p_prevprev;

        /* Store the computed numerator and advance state */
        P[len] = p_next;
        m   = m_next;
        d   = d_next;
        a_coef = a_next;
        ++len;               /* next index to compute */
    }

    return P[n];
}
