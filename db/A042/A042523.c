/*
 * A042523  Denominators of continued fraction convergents to sqrt(790).
 * https://oeis.org/A042523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042523: Denominators of continued fraction convergents to sqrt(790).
   Implemented using the standard continued‑fraction algorithm. */

/* Compile with `-DSEQ_T=<unsigned integer type>` or
   with an arbitrary‑precision integer class. */
#include <stdlib.h>
#include <stdint.h>

static const uint64_t D = 790;    /* D = 790. */
static const unsigned int a0 = 28;/* floor(sqrt(790)). */

/* Memoised cache of denominators.  The static variables hold the
   state of the continued‑fraction generation so that future calls
   can continue without recomputing earlier terms. */
static seq_t *cache = NULL;      /* Cache of q(i). */
static idx_t cache_len = 0;      /* Number of stored terms. */

/* Continued‑fraction state for generating the next partial quotient. */
static uint64_t cf_m = 0;        /* m_k. */
static uint64_t cf_d = 1;        /* d_k. */
static unsigned int cf_a_prev = a0; /* a_k (previous partial quotient). */

seq_t a(idx_t n)
{
    /* Initialise cache for the first term a(0)=1. */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;         /* OOM protection. */
        cache[0] = 1;                 /* q(0) = 1. */
        /* State (cf_m, cf_d, cf_a_prev) already set for producing a(1). */
    }

    /* Extend cache up to n if necessary. */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        /* Grow buffer. */
        if (cache) {
            cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        } else {
            cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        }
        if (!cache) return 0; /* OOM protection. */

        /* Compute new terms from old_len to n. */
        for (idx_t i = old_len; i <= n; ++i) {
            /* Generate next partial quotient a_i. */
            uint64_t m_next = cf_d * cf_a_prev - cf_m;
            uint64_t d_next = (D - m_next * m_next) / cf_d;
            unsigned int a_next = (unsigned int)((a0 + m_next) / d_next);

            /* Update continued‑fraction state. */
            cf_m = m_next;
            cf_d = d_next;
            cf_a_prev = a_next;

            /* Compute denominator q(i). */
            seq_t term = (seq_t)a_next * cache[i - 1];
            if (i >= 2) {
                term += cache[i - 2];
            }
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
