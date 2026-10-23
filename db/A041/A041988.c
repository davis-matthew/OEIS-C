/*
 * A041988  Numerators of continued fraction convergents to sqrt(517).
 * https://oeis.org/A041988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(517) */
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the framework. */
seq_t a(idx_t n)
{
    /* Continued‑fraction parameters for sqrt(517). */
    static uint64_t m = 0;         /* m_k */
    static uint64_t d = 1;         /* d_k */
    static uint64_t a_cf = 22;     /* a_k  (current coefficient) */
    const uint64_t D = 517;        /* discriminant */
    const uint64_t a0 = 22;        /* floor(sqrt(D)) */

    /* Memoised numerators p_k. */
    static seq_t *P = NULL;   /* array of numerators */
    static idx_t size = 0;    /* allocated size of P */
    static idx_t last = 0;    /* last computed index */

    /* Initialise on first call. */
    if (P == NULL) {
        size = 1;
        P = (seq_t *)calloc(size, sizeof(seq_t));
        if (P == NULL) return 0;       /* allocation failure */
        P[0] = (seq_t)a0;              /* p_0 = a0 = 22 */
        last  = 0;
    }

    /* Extend memoisation up to index n if required. */
    if (n > last) {
        /* Allocate or grow the array. */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(P, new_size * sizeof(seq_t));
        if (tmp == NULL) return P[last]; /* keep existing data on failure */
        P = tmp;
        size = new_size;

        /* Compute convergents from last+1 to n. */
        for (idx_t i = last + 1; i <= n; ++i) {
            /* Step the continued‑fraction recurrence. */
            m = d * a_cf - m;
            d = (D - m * m) / d;
            a_cf = (a0 + m) / d;   /* floor((a0 + m)/d) */

            /* Numerator recurrence: p_k = a_k * p_{k-1} + p_{k-2}. */
            seq_t p_prev1 = P[i - 1];
            seq_t p_prev2 = (i == 1) ? (seq_t)1 : P[i - 2];  /* p_{-1} = 1 for k=1 */
            P[i] = a_cf * p_prev1 + p_prev2;
        }
        last = n;
    }

    return P[n];
}
