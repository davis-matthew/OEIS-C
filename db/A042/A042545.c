/*
 * A042545  Denominators of continued fraction convergents to sqrt(801).
 * https://oeis.org/A042545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(801) */
#include <stdlib.h>

/* The function must have external linkage */
seq_t a(idx_t n)
{
    /* Constants for the continued fraction of sqrt(801) */
    const idx_t D   = 801ull;   /* the target number under the square root */
    const int  a0   = 28;       /* floor(sqrt(801)) */

    /* Static cache holding already computed denominators */
    static seq_t *den = NULL;
    static idx_t den_len = 0;          /* number of computed terms (size of the array) */

    /* State variables for the recurrence */
    static idx_t  m = 0, d = 1;        /* current m and d */
    static int    a_k = a0;            /* current partial quotient a_k */
    static seq_t  q_prev      = 1;     /* q_{k-1}  */
    static seq_t  q_prevprev  = 0;     /* q_{k-2}  */

    /* If we have not yet computed any terms, set up the initial state */
    if (den_len == 0) {
        /* Allocate for index 0 */
        den_len = 1;
        den = (seq_t*)malloc(sizeof(seq_t) * den_len);
        den[0] = 1;                   /* q_0 = 1 */
        /* State already set to initial values above */
    }

    /* Extend the cache if the requested index exceeds current capacity */
    if (n + 1 > den_len) {
        /* Allocate or reallocate the array to hold up to index n */
        seq_t *tmp = (seq_t*)realloc(den, sizeof(seq_t) * (n + 1));
        if (!tmp) {
            /* Allocation failed – abort; the calling environment
               will handle this unreasonably large request. */
            abort();
        }
        den = tmp;
        /* Compute missing terms from the current last index up to n */
        for (idx_t k = den_len; k <= n; ++k) {
            /* Compute the next continued fraction component */
            idx_t  m_next = d * a_k - m;
            idx_t  d_next = (D - m_next * m_next) / d;
            int    a_next = (int)((a0 + m_next) / d_next);

            /* Compute the corresponding denominator of the convergent */
            seq_t q_new = (seq_t)a_next * q_prev + q_prevprev;
            den[k] = q_new;

            /* Update the state for the next iteration */
            m            = m_next;
            d            = d_next;
            a_k          = a_next;
            q_prevprev   = q_prev;
            q_prev       = q_new;
        }
        /* Mark the new size of the cache */
        den_len = n + 1;
    }

    /* Return the cached denominator */
    return den[n];
}
