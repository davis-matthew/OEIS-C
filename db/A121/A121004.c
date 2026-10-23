/*
 * A121004  Numerators of partial sums of Catalan numbers scaled by powers of
 *          1/(5*5^2)=1/125.
 * https://oeis.org/A121004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of r(n) where r(n) = Sum_{k=0..n} Catalan(k) /
 *            5^(3*k) (in lowest terms)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"   /* catalan(idx_t k) – used only for its type */

/* Cached values for Catalan numbers and partial sums N(n) = a(n)*5^(3n). */
static seq_t *C_cache = NULL;        /* Catalan numbers */
static size_t C_cap   = 0;           /* number of cached Catalan entries */

static seq_t *N_cache = NULL;        /* partial sums including all 5‑factors */
static size_t N_cap   = 0;           /* number of cached N entries */

seq_t a(idx_t n)
{
    /* --- initialise caches --- */
    if (C_cap == 0) {
        C_cap = 1;
        C_cache = (seq_t *)malloc(C_cap * sizeof(seq_t));
        C_cache[0] = (seq_t)1;            /* Catalan(0) = 1 */
    }
    if (N_cap == 0) {
        N_cap = 1;
        N_cache = (seq_t *)malloc(N_cap * sizeof(seq_t));
        N_cache[0] = (seq_t)1;            /* N(0) = 1 as N = a * 5^(3n) */
    }

    /* --- grow Catalan cache up to n --- */
    if ((size_t)n + 1 > C_cap) {
        size_t new_cap = (size_t)n + 1;
        C_cache = (seq_t *)realloc(C_cache, new_cap * sizeof(seq_t));
        /* recurrence: C(k) = C(k-1) * 2*(2k-1) / (k+1) */
        for (size_t k = C_cap; k <= (size_t)n; ++k) {
            seq_t num = C_cache[k-1] * (seq_t)(2 * (2 * k - 1));
            seq_t denom = (seq_t)(k + 1);
            C_cache[k] = num / denom;        /* exact division */
        }
        C_cap = new_cap;
    }

    /* --- grow partial sum cache up to n --- */
    if ((size_t)n + 1 > N_cap) {
        size_t new_cap = (size_t)n + 1;
        N_cache = (seq_t *)realloc(N_cache, new_cap * sizeof(seq_t));
        for (size_t k = N_cap; k <= (size_t)n; ++k) {
            /* N(k) = 125 * N(k-1) + Catalan(k) */
            N_cache[k] = N_cache[k-1] * (seq_t)125 + C_cache[k];
        }
        N_cap = new_cap;
    }

    /* --- reduce N(n) by removing all factors of 5 to obtain a(n) --- */
    seq_t val = N_cache[(size_t)n];
    while ((val % (seq_t)5) == 0)
        val /= (seq_t)5;
    return val;
}
