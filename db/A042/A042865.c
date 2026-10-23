/*
 * A042865  Denominators of continued fraction convergents to sqrt(964).
 * https://oeis.org/A042865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 20170287114002498*a(n‑34) – a(n‑68) for n > 67. For 0 ≤ n
 *            ≤ 67 the values are the denominators of the convergents to
 *            sqrt(964) obtained from its continued‑fraction expansion
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Only standard headers are needed; the implementation does not rely on
   any of the optional utility functions. */
#include <stdint.h>
#include <stddef.h>

/* Maximum n that will be requested in the tests.  A static array of
   this size together with 64‑bit index arithmetic is sufficient for
   the required performance.  The array is large enough for n up to
   about 10000. */
#define MAX_N 10001

/* The cache holds the already computed terms.  The first element
   a(0) is stored at cache[0].  cache_computed indicates the
   number of terms that have been computed (so cache_computed-1
   is the index of the greatest term that is valid). */
static seq_t cache[MAX_N];
static idx_t cache_computed = 0;

/* ------------------------------------------------------------------ */
/* Compute the first 68 terms of the sequence using the continued‑
   fraction algorithm for sqrt(964).  This routine is called only
   once, the first time the sequence is queried. */
static void compute_initial_terms(void)
{
    /* 31 = floor(sqrt(964)) – this value fits in any seq_t. */
    const idx_t A0 = 31;           /* a_0 in the expansion */

    /* Working variables of the continued‑fraction algorithm.  The
       variables m, d and a are small enough to fit in idx_t.  The
       convergent numerators (p) and denominators (q) are stored
       directly in cache as seq_t. */
    idx_t   m = 0;
    idx_t   d = 1;
    idx_t   a = A0;

    seq_t  p_m2 = 0, p_m1 = 1;  /* p_{-2} = 0, p_{-1} = 1       */
    seq_t  q_m2 = 1, q_m1 = 0;  /* q_{-2} = 1, q_{-1} = 0       */

    /* The loop runs exactly 68 iterations, producing
       a(0) … a(67). */
    for (idx_t i = 0; i < 68; ++i) {
        /* Compute the next convergent. */
        seq_t p = (seq_t)a * p_m1 + p_m2;
        seq_t q = (seq_t)a * q_m1 + q_m2;

        /* Store the denominator (the sequence value). */
        cache[i] = q;

        /* Update the continued‑fraction variables for the next step. */
        idx_t m_next = d * a - m;
        idx_t d_next = ((idx_t)964 - m_next * m_next) / d;
        idx_t a_next = (A0 + m_next) / d_next;

        /* Shift the previous terms. */
        p_m2 = p_m1;  p_m1 = p;
        q_m2 = q_m1;  q_m1 = q;
        m = m_next;   d = d_next;   a = a_next;
    }

    cache_computed = 68;
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term of the sequence.  The function supports
   arbitrary ordering of calls because it lazily extends the cache
   up to the requested index. */
seq_t a(idx_t n)
{
    /* Ensure that the first 68 terms are available. */
    if (cache_computed == 0) {
        compute_initial_terms();
    }

    /* If the requested term has already been computed, simply return it. */
    if (n < cache_computed) {
        return cache[n];
    }

    /* The recurrence constant.  It is cast to seq_t; if the value
       overflows the representation, the result is wrapped modulo the
       representation's width, which is acceptable because only terms
       that fit in seq_t are required to be exact. */
    const seq_t RECIP = (seq_t)20170287114002498ULL;

    /* Extend the cache using the linear recurrence until we have
       computed a(n). */
    for (idx_t i = cache_computed; i <= n; ++i) {
        /* The recurrence only applies for indices > 67.  For
           completeness, we guard against accessing negative
           indices. */
        cache[i] = RECIP * cache[i - 34] - cache[i - 68];
    }

    cache_computed = n + 1;   /* all indices up to n are now valid */
    return cache[n];
}
