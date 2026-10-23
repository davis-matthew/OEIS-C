/*
 * A042466  Numerators of continued fraction convergents to sqrt(761).
 * https://oeis.org/A042466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of the continued *
 *            fraction of sqrt(761) (with a(0)=27)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed numerators and state variables for the
     * continued‑fraction algorithm.  All arithmetic is performed in
     * the smallest common integer type that can hold the values.
     * Small intermediate variables fit in uint64_t.       */
    static seq_t *cache = NULL;          /* p_n values */
    static size_t cache_cap = 0;         /* number of cached terms   */
    static seq_t prev2 = 1;              /* p_{k-2} (initially for k=0) */
    static seq_t prev1 = 27;             /* p_{k-1} (initially for k=0) */

    /* Continued‑fraction state after the last computed term.
     * These are needed to generate the next partial quotient
     * a_{k+1}.  They are defined for the first term (k=0). */
    static unsigned long long last_m = 0;    /* m_k   */
    static unsigned long long last_d = 1;    /* d_k   */
    static unsigned long long last_a = 27;   /* a_{k+1} for next term   */

    const unsigned long long D   = 761;      /* square‑free integer */
    const unsigned long long a0  = 27;       /* floor(sqrt(D))        */

    /* If this is the first call, initialise the cache and state. */
    if (cache_cap == 0) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        cache[0] = a0;                       /* p_0 = a0 */
        cache_cap = 1;

        /* Prepare state for generating a_1.   */
        unsigned long long m_next = last_d * last_a - last_m;          /* 32*27-0 = 27 */
        unsigned long long d_next = (D - m_next * m_next) / last_d;    /* (761-27^2)/1 = 32 */
        unsigned long long a_next = (a0 + m_next) / d_next;            /* (27+27)/32 = 1 */

        last_m = m_next;
        last_d = d_next;
        last_a = a_next;                     /* state ready for k = 1 */
    }

    /* Generate terms up to n.  The loop may run zero or more
     * iterations depending on the current cache size. */
    while (cache_cap <= n) {
        /* Compute the next numerator using the recurrence
         * p_k = a_k * p_{k-1} + p_{k-2}.  Here last_a holds a_k.  */
        seq_t p_curr = last_a * prev1 + prev2;

        /* Store the new numerator. */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cache[cache_cap] = p_curr;
        cache_cap++;

        /* Advance p_{k-2} and p_{k-1} for the next iteration. */
        prev2 = prev1;
        prev1 = p_curr;

        /* Compute the next partial quotient a_{k+1}. */
        unsigned long long m_next = last_d * last_a - last_m;
        unsigned long long d_next = (D - m_next * m_next) / last_d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        /* Update state for the following iteration. */
        last_m = m_next;
        last_d = d_next;
        last_a = a_next;
    }

    return cache[n];
}
