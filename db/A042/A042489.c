/*
 * A042489  Denominators of continued fraction convergents to sqrt(772).
 * https://oeis.org/A042489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042489: denominators of continued fraction convergents to sqrt(772)
   a(n) = k_n, with k_0 = 1, k_1 = a_1, and k_n = a_n * k_{n-1} + k_{n-2}
   where the partial quotients a_n form the continued fraction expansion of
   sqrt(772).  The expansion begins with a_0 = 27 and continues with
   a_1 = 1, a_2 = 3, a_3 = 1, a_4 = 1, a_5 = 1, a_6 = 5, ...
   The algorithm (see above) generates these a_n on demand and
   memoises all previously computed denominators.  The implementation
   is O(n) per call and works for any n ≥ 0. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /*  The constants for the continued fraction of sqrt(772).  */
    const idx_t D  = 772;          /* The radicand.      */
    const idx_t a0 = 27;           /* floor(sqrt(772)).  */

    /*  Static memoisation state.  */
    static seq_t  *denoms      = NULL;   /* Array of computed denominators. */
    static idx_t   denoms_cap  = 0;       /* Current capacity.                */
    static idx_t   denoms_size = 0;       /* Number of stored elements.       */
    static idx_t   m_cur       = 0;       /* Current m value in algorithm.    */
    static idx_t   d_cur       = 1;       /* Current d value in algorithm.    */
    static idx_t   a_cur       = a0;      /* Current a_k (with k = denoms_size - 1). */

    /* Initialise on first call.  */
    if (denoms == NULL)
    {
        /* Allocate space for a(0).  */
        denoms_cap = 1;
        denoms = (seq_t*)malloc(denoms_cap * sizeof(seq_t));
        if (denoms == NULL) return 0;     /* Out‑of‑memory – return 0.  */
        denoms[0] = (seq_t)1;              /* a(0) = 1.                */
        denoms_size = 1;
    }

    /* Extend the cache up to n if necessary.  */
    if (n >= denoms_size)
    {
        /* Ensure enough capacity.  */
        if (denoms_cap < n + 1)
            ensure_capacity(n + 1, &denoms_cap, (void**)&denoms, sizeof(seq_t));

        /* Compute remaining terms.  */
        for (idx_t i = denoms_size; i <= n; ++i)
        {
            /* Generate a_{i} using the continued‑fraction algorithm.  */
            idx_t m_next = d_cur * a_cur - m_cur;
            idx_t d_next = (D - m_next * m_next) / d_cur;
            idx_t a_next = (a0 + m_next) / d_next;

            /* Compute the denominator k_i.  */
            seq_t prev1 = denoms[i - 1];
            seq_t prev2 = (i >= 2) ? denoms[i - 2] : (seq_t)0;
            seq_t denom = (seq_t)a_next * prev1 + prev2;

            /* Store the result.  */
            denoms[i] = denom;
            ++denoms_size;

            /* Advance the state for the next step.  */
            m_cur = m_next;
            d_cur = d_next;
            a_cur = a_next;
        }
    }

    /* Return the cached value.  */
    return denoms[(size_t)n];
}
