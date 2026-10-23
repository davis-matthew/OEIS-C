/*
 * A042847  Denominators of continued fraction convergents to sqrt(954).
 * https://oeis.org/A042847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_capacity_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(954). */
/* Computed by the standard recurrence for continued fraction convergents. */
/* a(0) = 1, a(1) = 1, ... [full sequence as defined in OEIS A042847] */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_capacity_idx_t.h"

static void ensure_capacity_seq_t(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* constants for sqrt(954) */
    const idx_t N   = 954;
    const idx_t a0  = 30;                    /* floor(sqrt(954)) */

    /* static state for memoisation */
    static seq_t *den_cache      = NULL;
    static size_t den_cap        = 0;
    static size_t den_len        = 0;

    static idx_t *a_cache        = NULL;
    static size_t a_cap          = 0;
    static size_t a_len          = 0;

    /* continued fraction state */
    static idx_t m   = 0;          /* kernel term m_k */
    static idx_t d   = 1;          /* denominator term d_k */
    static idx_t a_k = a0;         /* current coefficient a_k */

    static bool initialized = false;
    if (!initialized) {
        /* initialise first term a0 and denominators q0 */
        ensure_capacity_seq_t(&den_cache, &den_cap, 1);
        den_cache[0] = 1;          /* q0 = 1 */
        den_len = 1;

        ensure_capacity_idx_t(&a_cache, &a_cap, 1);
        a_cache[0] = a0;           /* a0 = 30 */
        a_len = 1;

        /* m, d, a_k already as a0; next generation will use them */
        initialized = true;
    }

    size_t target = (size_t)n;
    if (target < den_len) {
        /* already computed */
        return den_cache[target];
    }

    /* set up previous two denominators for recurrence */
    seq_t prev1 = (den_len > 0) ? den_cache[den_len - 1] : 0;
    seq_t prev2 = (den_len > 1) ? den_cache[den_len - 2] : 0;

    /* generate terms up to n */
    for (size_t k = den_len; k <= target; ++k) {
        /* compute next continued fraction coefficient a_k for this k */
        if (k == 0) {
            /* a0 already known */
        } else {
            /* recurrence for m, d, a_k */
            idx_t m_next = d * a_k - m;
            idx_t d_next = (N - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a_k = a_next;
        }

        /* store a_k in cache */
        ensure_capacity_idx_t(&a_cache, &a_cap, a_len + 1);
        a_cache[a_len++] = a_k;

        /* compute denominator q_k = a_k * prev1 + prev2 */
        seq_t q_k = a_k * prev1 + prev2;

        /* store denominator */
        ensure_capacity_seq_t(&den_cache, &den_cap, den_len + 1);
        den_cache[den_len++] = q_k;

        /* shift previous denominators for next iteration */
        prev2 = prev1;
        prev1 = q_k;
    }

    return den_cache[target];
}
