/*
 * A042442  Numerators of continued fraction convergents to sqrt(749).
 * https://oeis.org/A042442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of continued fraction of
 *            sqrt(749)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 17, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* extern types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    static seq_t *p_cache = NULL;
    static size_t p_cap   = 0;
    static idx_t p_len    = 0;   /* number of terms currently cached (next index to compute) */

    /* Continued‑fraction generation state for sqrt(749) */
    static const idx_t D   = 749;
    static const int  a0   = 27;  /* floor(sqrt(749)) */
    static idx_t m = 0;
    static idx_t d = 1;
    static idx_t a_cur = a0;

    /* Recurrence variables for p_{k-1} and p_{k-2} */
    static seq_t p_prev2 = 1;  /* p_{-1} */
    static seq_t p_prev1 = a0;/* p_{0} */

    /* Initialise on first call */
    if (p_len == 0) {
        /* Allocate cache for at least one element */
        p_cap = 1;
        p_cache = (seq_t *)malloc(p_cap * sizeof(seq_t));
        if (!p_cache) return (seq_t)0; /* Could handle error, but not needed for sequences */
        p_cache[0] = a0;
        p_len = 1;
    }

    /* Extend cache up to n if necessary */
    if (n >= p_len) {
        /* Ensure cache has capacity */
        if (p_cap < (size_t)(n + 1)) {
            size_t new_cap = p_cap ? p_cap * 2 : 4;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* Allocation failure handling */
            p_cache = tmp;
            p_cap = new_cap;
        }

        /* Compute terms p_k for k = p_len .. n */
        for (idx_t k = p_len; k <= n; ++k) {
            /* Next continued‑fraction coefficient a_k */
            idx_t m_next = d * a_cur - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            /* Update state for next iteration */
            m = m_next; d = d_next; a_cur = a_next;

            /* Convergent numerator recurrence:  p_k = a_k * p_{k-1} + p_{k-2} */
            seq_t p_k = (seq_t)a_next * p_prev1 + p_prev2;
            p_cache[k] = p_k;

            /* Shift previous values for next step */
            p_prev2 = p_prev1;
            p_prev1 = p_k;
        }

        p_len = n + 1;
    }

    return p_cache[n];
}
