/*
 * A042522  Numerators of continued fraction convergents to sqrt(790).
 * https://oeis.org/A042522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(790) */

/* This function computes a(n), the numerator of the n‑th
   convergent to sqrt(790).  The continued fraction of sqrt(790)
   has period 20.  The implementation uses the standard
   recurrence for the convergents and memoises results in a
   static array so that subsequent calls are O(1). */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* The continued‑fraction data are the same for
       all calls, so we keep them in static variables. */
    static uint64_t D = 790ULL;           /* D in sqrt(D)          */
    static const uint64_t a0 = 28ULL;     /* floor(sqrt(790))      */
    static uint64_t m = 0;                /* current m_k           */
    static uint64_t d = 1;                /* current d_k           */
    static uint64_t a_prev = a0;          /* a_k for next step     */

    /* Convergent numerators: p_{k-1} and p_{k-2}.  Initially
       p_{-1}=1, p_{-2}=0. */
    static seq_t p_prev1 = 1;             /* p_{k-1}               */
    static seq_t p_prev2 = 0;             /* p_{k-2}               */

    /* Memoisation cache: store computed numerators. */
#if defined(__cplusplus)
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;           /* array of numerators    */
    static size_t cache_cap = 0;          /* allocated capacity     */
#endif
    static size_t cache_size = 0;         /* number of computed args*/

    /* Allocate or grow cache if necessary */
#if defined(__cplusplus)
    if (cache.size() < (size_t)(n + 1)) {
        cache.resize(n + 1);
    }
#else
    if (!cache) {
        cache_cap = 16384;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
#endif

    /* Compute values up to the requested index */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t p;                       /* numerator for index i */

        if (i == 0) {
            /* First convergent */
            p = (seq_t)a0;              /* a0 * p_prev1 + p_prev2 (p_prev1=1, p_prev2=0) */
        } else {
            /* Determine next continued‑fraction coefficient a_i */
            uint64_t m_next = d * a_prev - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_cur  = (a0 + m_next) / d_next;

            /* Convergent numerator recurrence */
            p = ((seq_t)a_cur) * p_prev1 + p_prev2;

            /* Update continued‑fraction state */
            m      = m_next;
            d      = d_next;
            a_prev = a_cur;
        }

        /* Store the result in the cache */
#if defined(__cplusplus)
        cache[i] = p;
#else
        cache[i] = p;
#endif

        /* Rotate the p_prev* values for the next iteration */
        seq_t old_p_prev1 = p_prev1;
        p_prev1 = p;            /* new p_prev1 is current numerator   */
        p_prev2 = old_p_prev1;  /* new p_prev2 is previous numerator  */
    }

    /* Update number of computed entries */
    cache_size = (size_t)n + 1;

    /* Return the requested term */
    return
#if defined(__cplusplus)
        cache[n];
#else
        cache[n];
#endif
}
