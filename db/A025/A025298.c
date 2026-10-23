/*
 * A025298  Numbers that are the sum of 2 nonzero squares in 7 or more ways.
 * https://oeis.org/A025298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   _integer_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: A025298 is defined as the sequence of positive integers
   that can be expressed as a sum of two non‑zero squares in at least
   seven distinct unordered ways.  The following routine performs a
   simple sieve: it iterates over all pairs (a,b) with a,b≥1, counts the
   number of representations for each sum s=a²+b², and collects those
   s whose count is at least 7.  The search limit is increased
   geometrically until the required amount of terms is found.  The
   function is fully memoised so that calls with arbitrary n are
   answered quickly.  No hard‑coded values are used. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_integer_isqrt.h"

/* ----------  Global state for memoisation  ---------- */
static idx_t       g_limit          = 0;     /* current search limit */
static uint8_t     *g_counts         = NULL; /* temporary representation count */
static seq_t      *g_terms          = NULL; /* cached sequence terms */
static idx_t       g_terms_count    = 0;     /* number of cached terms */

/* ----------  Build the sequence up to 'needed' terms  ---------- */
static void build_terms(idx_t needed)
{
    if (g_terms_count >= needed) return;          /* already enough */

    /* Free previous data (if any) to start fresh.  Alternate
       approaches could incrementally extend counts, but a full
       recomputation is simpler and fast enough for n≈10000.   */
    if (g_counts) free(g_counts);                  g_counts = NULL;
    if (g_terms)  free(g_terms);                   g_terms  = NULL;
    g_terms_count = 0;

    /* Starting search limit – big enough to contain many terms. */
    idx_t limit = (g_limit == 0) ? 50000 : g_limit;

    while (1) {
        g_limit = limit;

        /* Allocate count array (zeroed) – cast from void*. */
        g_counts = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
        if (!g_counts) abort();                    /* memory failure */

        /* Pre‑compute upper bound for a. */
        idx_t max_a = _integer_isqrt(limit);

        /* Count unordered sums a^2 + b^2 with 1 ≤ a ≤ b. */
        for (idx_t a = 1; a <= max_a; ++a) {
            idx_t a2 = a * a;
            for (idx_t b = a; ; ++b) {
                idx_t s = a2 + b * b;
                if (s > limit) break;
                if (g_counts[(size_t)s] < 255) /* avoid overflow */
                    ++g_counts[(size_t)s];
            }
        }

        /* Allocate initial capacity for terms.  20000 is a reasonable
           upper bound for the first 10000 terms; it will grow if
           necessary.  */
        size_t cap = (needed > 20000) ? needed : 20000;
        g_terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!g_terms) abort();                      /* memory failure */

        /* Collect all numbers with at least 7 representations. */
        for (idx_t s = 1; s <= limit; ++s) {
            if (g_counts[(size_t)s] >= 7) {
                if (g_terms_count == cap) {
                    /* Double the capacity. */
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(g_terms, cap * sizeof(seq_t));
                    if (!tmp) abort();            /* memory failure */
                    g_terms = tmp;
                }
                g_terms[g_terms_count++] = (seq_t)s;
                if (g_terms_count >= needed) break;
            }
        }

        /* Clean up counts before checking if we have enough terms. */
        free(g_counts);
        g_counts = NULL;

        if (g_terms_count >= needed) return;       /* success */

        /* Not enough terms – increase the search limit and repeat. */
        free(g_terms);
        g_terms = NULL;
        g_terms_count = 0;
        limit <<= 1;                               /* double limit */
    }
}

/* ----------  Public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* undefined, safe fallback */
    build_terms(n);
    return g_terms[n - 1];
}
