/*
 * A041913  Denominators of continued fraction convergents to sqrt(478).
 * https://oeis.org/A041913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of sqrt(478)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the continued‑fraction coefficients of
   sqrt(478) once, then generate all denominators by the standard
   recurrence q_k = a_k * q_{k-1} + q_{k-2} where
   a_0 = ⌊√478⌋ = 21 and the other a_k repeat with period `period`. */

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- global cache for denominators ----------------- */
static seq_t *q_cache   = NULL;   /* array of denominators q_k  */
static idx_t  cache_len = 0;      /* number written in q_cache  */
static size_t cache_cap = 0;      /* capacity of q_cache        */

/* ---------- cache for continued‑fraction coefficients -------- */
static seq_t *cf_coeff       = NULL;   /* a_1 , a_2 , … , a_period   */
static size_t period          = 0;      /* length of the period        */
static bool   cf_ready        = false;  /* true after cf_coeff is ready */

/* ---------- constants for sqrt(478) ------------------------- */
static const uint64_t D  = 478;         /* value under the square root   */
static const uint64_t A0 = 21;          /* floor(sqrt(D))                */

/* ---------- helper: ensure cache capacity ------------------- */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= (idx_t)cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while ((idx_t)new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(q_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                  /* OOM – stop execution */
    q_cache = tmp;
    cache_cap = new_cap;
}

/* ---------- helper: generate continued‑fraction coeffs ----- */
static void init_cf_coeff(void)
{
    if (cf_ready) return;                /* already done */

    /* Dynamically build the period.  We first compute the next
       partial quotient, store it, and stop when it equals 2*A0
       (the known termination condition for sqrt(D)).          */

    uint64_t m = 0, d = 1;
    uint64_t a = A0;                     /* the a0 term, not stored */
    size_t   idx = 0;                    /* number of coefficients gathered */

    size_t cap = 32;
    cf_coeff = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!cf_coeff) abort();

    while (true) {
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (A0 + m_next) / d_next;   /* floor division */

        /* store the coefficient a_next (this is a_(idx+1) ) */
        if (idx >= cap) {
            cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cf_coeff, cap * sizeof(seq_t));
            if (!tmp) abort();
            cf_coeff = tmp;
        }
        cf_coeff[idx++] = (seq_t)a_next;

        if (a_next == 2 * A0) break;       /* period ends */

        /* move to next step */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    period = idx;
    cf_ready = true;
}

/* ---------- main sequence function ------------------------ */
seq_t a(idx_t n)
{
    /* Ensure that continued‑fraction coefficients are known. */
    init_cf_coeff();

    /* Extend denominator cache up to index n, if necessary. */
    if (cache_len <= n) {
        /* Determine starting point for new computation. */
        seq_t prev2;                           /* q_{k-2}   (k = start)   */
        seq_t prev1;                           /* q_{k-1}   (k = start)   */
        idx_t k_start;                         /* next index to compute */

        if (cache_len == 0) {                  /* no values yet */
            prev2 = (seq_t)1;                  /* q_{-2} = 1   */
            prev1 = (seq_t)0;                  /* q_{-1} = 0   */
            k_start = 0;
        } else if (cache_len == 1) {           /* only q0 stored */
            prev2 = (seq_t)0;                  /* q_{-1} = 0   */
            prev1 = q_cache[0];                /* q0           */
            k_start = 1;
        } else {
            prev2 = q_cache[cache_len - 2];
            prev1 = q_cache[cache_len - 1];
            k_start = cache_len;
        }

        /* Ensure capacity now that we know the final needed size. */
        ensure_cache_cap(n + 1);

        for (idx_t k = k_start; k <= n; ++k) {
            seq_t a_k;   /* partial quotient for index k  */

            if (k == 0) {
                a_k = (seq_t)A0;               /* a0 for q0 */
            } else {
                size_t idx = (size_t)(k - 1) % period;
                a_k = cf_coeff[idx];
            }

            seq_t q_curr = a_k * prev1 + prev2;  /* recurrence q_k */

            q_cache[k] = q_curr;

            /* shift for next iteration */
            prev2 = prev1;
            prev1 = q_curr;
        }

        cache_len = n + 1;  /* now values up to n are cached */
    }

    return q_cache[n];
}
