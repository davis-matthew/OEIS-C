/*
 * A041905  Denominators of continued fraction convergents to sqrt(474).
 * https://oeis.org/A041905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 27, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 198 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041905
   Denominators of continued fraction convergents to sqrt(474)
   Formula: a(n) = 387098*a(n-14) - a(n-28), for n > 27
   Initial values: a(0) = 1, a(1) = 1, a(2) = 4, a(3) = 9, a(4) = 13, a(5) = 22,
   a(6) = 35, a(7) = 232, a(8) = 267, a(9) = 499, a(10) = 766,
   a(11) = 2031, a(12) = 6859, a(13) = 8890, a(14) = 380239,
   a(15) = 389129, a(16) = 1547626, a(17) = 3484381, a(18) = 5032007,
   a(19) = 8516388, a(20) = 13548395, a(21) = 89806758,
   a(22) = 103355153, a(23) = 193161911, a(24) = ...
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute the first 28 denominators by the standard continued‑fraction
   algorithm for sqrt(474).  The algorithm uses only integer arithmetic. */
static void init_first_28(seq_t first[28])
{
    const seq_t D = (seq_t)474;          /* non‑square  */
    const seq_t a0 = (seq_t)21;         /* floor(sqrt(474)) */

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    seq_t p_m2 = (seq_t)0;  /* p[-2] */
    seq_t p_m1 = (seq_t)1;  /* p[-1] */
    seq_t q_m2 = (seq_t)1;  /* q[-2] */
    seq_t q_m1 = (seq_t)0;  /* q[-1] */

    for (idx_t i = 0; i < 28; ++i) {
        seq_t p = a * p_m1 + p_m2;
        seq_t q = a * q_m1 + q_m2;
        first[(size_t)i] = q;             /* denominator of i‑th convergent */

        /* shift previous values  */
        p_m2 = p_m1;   p_m1 = p;
        q_m2 = q_m1;   q_m1 = q;

        /* next triple (m, d, a) of the continued fraction */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = ((seq_t)a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *cache      = NULL;   /* array of computed values   */
    static size_t cache_size = 0;      /* number of values stored    */
    static size_t cache_cap  = 0;      /* allocated capacity         */

    /* initial setup: compute the first 28 values once */
    if (!initialized) {
        static seq_t first_28[28];
        init_first_28(first_28);

        cache_cap  = 28;
        cache_size = 28;
        cache      = mem_new(seq_t, cache_cap);
        if (!cache) {
            /* allocation failure – return 0 (should not happen in tests) */
            return (seq_t)0;
        }
        mem_copy(cache, first_28, 28 * sizeof(seq_t));
        initialized = true;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < need) {
            new_cap <<= 1;
        }
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            /* allocation failure – return 0 (should not happen in tests) */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence
       a(k) = 387098*a(k-14) - a(k-28) */
    const seq_t COEF = (seq_t)387098;
    for (idx_t k = cache_size; k <= n; ++k) {
        cache[(size_t)k] = COEF * cache[(size_t)(k - 14)]
                           - cache[(size_t)(k - 28)];
    }

    cache_size = need;
    return cache[(size_t)n];
}
