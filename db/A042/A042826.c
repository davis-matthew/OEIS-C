/*
 * A042826  Numerators of continued fraction convergents to sqrt(944).
 * https://oeis.org/A042826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(944) – computed
   using the standard recurrence with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static bool initialised = false;
    static seq_t *p = NULL;      /* numerators */
    static seq_t *a_ = NULL;     /* partial quotients */
    static size_t p_cap = 0;
    static size_t p_len = 0;
    static seq_t cur_m = 0, cur_d = 1, cur_a = 0;  /* state of CF generator */

    const seq_t D  = 944;   /* discriminant */
    const seq_t a0 = 30;    /* floor(sqrt(944)) */

    if (!initialised) {
        p_len = 1;
        p_cap = 1;
        p  = (seq_t*)malloc(sizeof(seq_t)*p_cap);
        a_ = (seq_t*)malloc(sizeof(seq_t)*p_cap);
        p[0]   = a0;        /* a(0) */
        a_[0]  = a0;
        cur_m = 0;
        cur_d = 1;
        cur_a = a0;
        initialised = true;
    }

    /* Expand cache up to required index n */
    while (p_len <= n) {
        /* Generate next partial quotient a_k */
        seq_t m_next = cur_d * cur_a - cur_m;
        seq_t d_next = (D - m_next * m_next) / cur_d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Resize if necessary */
        if (p_len >= p_cap) {
            p_cap = p_cap ? p_cap * 2 : 1;
            p  = (seq_t*)realloc(p,  sizeof(seq_t) * p_cap);
            a_ = (seq_t*)realloc(a_, sizeof(seq_t) * p_cap);
        }

        a_[p_len] = a_next;

        /* Compute numerator p_k using recurrence p_k = a_k * p_{k-1} + p_{k-2} */
        if (p_len == 1) {                      /* k = 1 */
            p[1] = a_next * p[0] + 1;
        } else {                               /* k >= 2 */
            p[p_len] = a_next * p[p_len-1] + p[p_len-2];
        }

        /* Advance state for the next coefficient */
        cur_m = m_next;
        cur_d = d_next;
        cur_a = a_next;

        p_len++;   /* now we have computed up to index p_len-1 */
    }

    return p[n];
}
