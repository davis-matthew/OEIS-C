/*
 * A041070  Numerators of continued fraction convergents to sqrt(42).
 * https://oeis.org/A041070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041070: numerators of continued‑fraction convergents to sqrt(42) */
/* Computes a(n) for any n ≥ 0, memoising results for efficiency. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---------- static state for memoisation and continued‑fraction generation ---------- */
    static seq_t *cache      = NULL;   /* array of computed numerators */
    static size_t cap       = 0;       /* capacity of the array */
    static size_t len       = 0;       /* number of values stored (len == 0 => uninitialised) */

    /* continued‑fraction state:  m, d, a_cur are idx_t (uint64_t) */
    static idx_t  m          = 0,      /* m_i in the Lagrange algorithm */
                  d          = 1,      /* d_i */
                  a_cur      = 6;      /* a_i, initial a0 = floor(sqrt(42)) */
    static const idx_t D     = 42;     /* the square‑free integer under the root */
    static const idx_t a0    = 6;      /* floor(sqrt(42)) */

    /* recurrence for the numerators of convergents:  p_-1 = 1, p_0 = a0  */
    static seq_t  p_minus2   = 1,      /* p_{i-2}  */
                  p_minus1   = 6;      /* p_{i-1}  */

    /* ---------- initialise storage ---------- */
    if (len == 0) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure → return 0 */
        cache[0] = (seq_t)a0;
        len = 1;
    }

    /* ---------- grow cache if necessary ---------- */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* ---------- generate terms up to index n (inclusive) ---------- */
    while (len <= (size_t)n) {
        /* Compute next partial quotient a_next using the Lagrange algorithm */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Compute next numerator using the recurrence p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t p_next = ((seq_t)a_next) * p_minus1 + p_minus2;

        /* Store the new numerator and shift the state for the next iteration */
        cache[len] = p_next;

        p_minus2 = p_minus1;
        p_minus1 = p_next;

        /* Update continued‑fraction state */
        m     = m_next;
        d     = d_next;
        a_cur = a_next;

        ++len;
    }

    return cache[(size_t)n];
}
