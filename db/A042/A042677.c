/*
 * A042677  Denominators of continued fraction convergents to sqrt(868).
 * https://oeis.org/A042677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 7688126*a(n-8) - a(n-16) for n>15. The first 16 terms are
 *            obtained from the convergents of sqrt(868)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the first 16 denominators as convergents of sqrt(868). */
static void compute_initial_terms(seq_t *arr, size_t count)
{
    const idx_t D = 868;
    /* floor(sqrt(D)) */
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) {
        ++a0;
    }

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;
    seq_t q_prevprev = 1;
    seq_t q_prev     = 0;

    for (size_t k = 0; k < count; ++k) {
        seq_t p = (seq_t)a * p_prev + p_prevprev;
        seq_t q = (seq_t)a * q_prev + q_prevprev;
        arr[k] = q;                       /* store denominator */

        /* Next continued‑fraction coefficient */
        idx_t m_next = (idx_t)((seq_t)d * a - m);
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        p_prevprev = p_prev;
        p_prev = p;
        q_prevprev = q_prev;
        q_prev = q;
    }
}

/* Static cache for memoization */
static seq_t *CACHE   = NULL;
static size_t CACHE_LEN = 0;
static size_t CACHE_CAP = 0;

/* Ensure that CACHE contains values 0 … n. */
static void ensure_cache(idx_t n)
{
    if (CACHE_LEN > (size_t)n) {
        return;
    }

    size_t needed = (size_t)n + 1;
    if (needed > CACHE_CAP) {
        size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 64;
        if (new_cap < needed) {
            new_cap = needed;
        }
        seq_t *new_cache = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
        CACHE = new_cache;
        CACHE_CAP = new_cap;
    }

    /* Compute the first 16 terms if not yet done. */
    if (CACHE_LEN < 16) {
        size_t to_compute = 16;
        if (to_compute > needed) {
            to_compute = needed;
        }
        compute_initial_terms(CACHE, to_compute);
        CACHE_LEN = to_compute;
    }

    /* Compute further terms using the recurrence relation. */
    for (size_t i = CACHE_LEN; i <= (size_t)n; ++i) {
        if (i < 16) { /* already filled by compute_initial_terms */
            continue;
        }
        CACHE[i] = (seq_t)7688126 * CACHE[i - 8] - CACHE[i - 16];
    }
    CACHE_LEN = needed;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return CACHE[(size_t)n];
}
