/*
 * A172074  Continued fraction expansion of sqrt(12500)+50 = 100*phi, where
 *          phi=(sqrt(5)+1)/2 is the golden ratio.
 * https://oeis.org/A172074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the continued‑fraction expansion of sqrt(12500)+50
   (which equals 100*phi).  The terms are produced by the standard
   recurrence for a quadratic surd (P+sqrt(D))/Q:
      a_k  = floor((S + P)/Q);
      P'   = a_k * Q - P;
      Q'   = (D - P'^2) / Q;
   where D = 12500 and S = floor(sqrt(D)) = 111.  The function
   caches the sequence so that any order of calls is supported. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Surd constants */
static const uint64_t D = 12500ull;     /* the integer under the square root */
static const uint64_t S = 111ull;      /* floor(sqrt(D)) */

/* Cache for computed terms */
static seq_t *cached = NULL;   /* array of terms */
static size_t cap    = 0;      /* allocated capacity */
static size_t len    = 0;      /* number of terms stored */

/* Current state of the recurrence (P, Q) after `len` terms have been
   generated.  Initial state corresponds to the surd (50 + sqrt(12500))/1. */
static uint64_t P = 50ull;   /* numerator offset */
static uint64_t Q = 1ull;    /* denominator */

/* Ensure that cached[n] exists (i.e., the cache contains terms 0..n). */
static void ensure_cached(idx_t n) {
    /* Lazy allocation of the cache array. */
    if (len == 0) {
        cap = 8;
        cached = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until the requested index is filled. */
    while (len <= (size_t)n) {
        /* a_k = floor((S + P) / Q) */
        uint64_t a_val = (S + P) / Q;

        /* Grow the cache array if necessary. */
        if (len == cap) {
            size_t new_cap = cap << 1;
            seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
            cached = tmp;
            cap = new_cap;
        }

        /* Store the computed term. */
        cached[len++] = (seq_t)a_val;

        /* Update the P, Q state for the next iteration. */
        uint64_t newP = a_val * Q - P;
        uint64_t newQ = (D - newP * newP) / Q;
        P = newP;
        Q = newQ;
    }
}

/* Return the n‑th term of the sequence, with a(0) = 161. */
seq_t a(idx_t n) {
    ensure_cached(n);
    return cached[(size_t)n];
}
