/*
 * A042050  Numerators of continued fraction convergents to sqrt(549).
 * https://oeis.org/A042050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 3532638098*a(n-18) - a(n-36) with initial terms
 *            a(0)...a(35) given by the convergents to sqrt(549)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 169 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ----------------------------------------------------------------
   Compute the first 36 convergent numerators of sqrt(549).
   This uses the classic continued‑fraction algorithm.
   ---------------------------------------------------------------- */
static void init_36_terms(seq_t *cache)
{
    const int64_t D   = 549;
    const int64_t a0  = 23;           /* floor(sqrt(549)) */

    int64_t m = 0;
    int64_t d = 1;
    int64_t a_cf = a0;                /* Current continued‑fraction term */

    seq_t p_prev2 = 0;                /* p_{-2} */
    seq_t p_prev1 = 1;                /* p_{-1} */

    for (idx_t i = 0; i < 36; ++i) {
        /* p_k = a_cf * p_{k-1} + p_{k-2} */
        seq_t p = (seq_t)a_cf * p_prev1 + p_prev2;
        cache[i] = p;

        /* Update for next step in the continued fraction */
        m = d * a_cf - m;
        d = (D - m * m) / d;
        a_cf = (a0 + m) / d;

        /* Shift indices for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
}

/* ----------------------------------------------------------------
   Sequence access function.
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache  = NULL;      /* cached terms */
    static size_t cap    = 0;         /* allocated size of cache      */
    static idx_t computed = 0;        /* next index not yet computed  */

    /* ------------------------------------------------------------
       1) Ensure we have space for indices [0 .. n]
       ------------------------------------------------------------ */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        /* increase a bit to reduce reallocs */
        if (newcap < 64) newcap = 64;
        else newcap <<= 1;
        if (cache == NULL) {
            cache = mem_new(seq_t, newcap);
        } else {
            cache = mem_resize(seq_t, cache, newcap);
        }
        /* initialise new slots to zero – not strictly needed */
        if (cache) {
            for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        }
        cap = newcap;
    }

    /* ------------------------------------------------------------
       2) Initialise the first 36 terms once.
       ------------------------------------------------------------ */
    if (computed == 0) {
        if (cap < 36) {
            /* grow to at least 36 */
            size_t newcap = 64;
            if (cache == NULL) cache = mem_new(seq_t, newcap);
            else cache = mem_resize(seq_t, cache, newcap);
            cap = newcap;
        }
        init_36_terms(cache);
        computed = 36; /* indices 0..35 are valid */
    }

    /* ------------------------------------------------------------
       3) Return a cached value if we already have it.
       ------------------------------------------------------------ */
    if (n < computed) {
        return cache[n];
    }

    /* ------------------------------------------------------------
       4) Compute remaining terms using linear recurrence.
          a(n) = 3532638098 * a(n-18) - a(n-36)
       ------------------------------------------------------------ */
    const seq_t COEFF = (seq_t)3532638098ULL;
    for (idx_t i = computed; i <= n; ++i) {
        seq_t prod = COEFF * cache[i - 18];
        cache[i] = prod - cache[i - 36];
    }
    computed = n + 1;

    return cache[n];
}
