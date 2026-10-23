/*
 * A042356  Numerators of continued fraction convergents to sqrt(705).
 * https://oeis.org/A042356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(705). * The
 *            convergents p_k/q_k of a quadratic irrational satisfy the *
 *            recurrence: * p_-2 = 0, p_-1 = 1 * q_-2 = 1, q_-1 = 0 * p_k =
 *            a_k * p_(k-1) + p_(k-2) * q_k = a_k * q_(k-1) + q_(k-2) * * For
 *            sqrt(D) the continued fraction coefficients a_k are...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *P = NULL;      /* numerators */
    static seq_t *Q = NULL;      /* denominators */
    static size_t cap = 0;       /* allocated capacity */
    static size_t len = 0;       /* number of computed elements */
    static idx_t m = 0;          /* current m_k in the CF algorithm */
    static idx_t d = 1;          /* current d_k */
    static idx_t cur_a = 26;     /* a_k for the last computed index (a_0) */

    /* initialise on first call */
    if (len == 0) {
        size_t initial = 1;
        P = (seq_t *)malloc(initial * sizeof(seq_t));
        Q = (seq_t *)malloc(initial * sizeof(seq_t));
        if (!P || !Q) abort();   /* allocation failure */
        cap = initial;
        /* first convergent: a0 = ⌊√705⌋ = 26 */
        P[0] = (seq_t)26;
        Q[0] = (seq_t)1;
        len = 1;
        /* state already set: m=0, d=1, cur_a=26 */
    }

    if (n < (idx_t)len)
        return P[(size_t)n];

    /* ensure capacity for up to n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *newP = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
        seq_t *newQ = (seq_t *)realloc(Q, new_cap * sizeof(seq_t));
        if (!newP || !newQ) abort();
        P = newP;
        Q = newQ;
        cap = new_cap;
    }

    /* compute missing convergents up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* next CF coefficients */
        idx_t next_m = d * cur_a - m;
        idx_t next_d = (705 - next_m * next_m) / d;
        idx_t next_a = (26 + next_m) / next_d;

        /* compute p_i and q_i */
        seq_t p, q;
        if (i == 1) {
            /* special case: uses p_-1 = 1, q_-1 = 0 */
            p = (seq_t)next_a * P[0] + 1;    /* p_-1 = 1 */
            q = (seq_t)next_a * Q[0];        /* q_-1 = 0 */
        } else {
            p = (seq_t)next_a * P[i-1] + P[i-2];
            q = (seq_t)next_a * Q[i-1] + Q[i-2];
        }

        P[i] = p;
        Q[i] = q;

        /* update state for next iteration */
        m = next_m;
        d = next_d;
        cur_a = next_a;
    }
    len = n + 1;
    return P[(size_t)n];
}
