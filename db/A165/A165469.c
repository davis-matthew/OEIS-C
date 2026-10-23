/*
 * A165469  Numbers of the form 4n+3 for which Sum_{i=1..u} J(i,4n+3) is never
 *          zero for any u in range [1,(2n+1)], where J(i,m) is the Jacobi
 *          symbol.
 * https://oeis.org/A165469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A165469: a(n) = the n-th odd integer 4k+3 for which
 *  ∑_{i=1}^u Jacobi(i,4k+3) ≠ 0 for every u ∈ [1,(4k+3-1)/2].
 *  This implementation searches forward, testing the partial sums
 *  until the required number of terms is found.  It caches all terms
 *  so that subsequent calls are fast, and works for any order of
 *  requests.  The search stops early when a zero partial sum is
 *  encountered, keeping the runtime very low for moderate n.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../utils/jacobi_symbol.h"
#ifdef __cplusplus
}
#endif

/* Static cache of computed terms */
static seq_t *terms = NULL;      /* array of terms */
static size_t term_count = 0;    /* number of stored terms */
static size_t term_cap   = 0;    /* capacity of terms array */
static uint64_t next_candidate = 3;   /* next m ≡ 3 (mod 4) to test */

/* Ensure the cache holds at least n terms (n is 1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;               /* nothing to compute */

    /* No cache yet: create with an initial capacity. */
    if (terms == NULL) {
        term_cap = 64;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    }

    /* Generate terms until we reach the requested count. */
    while (term_count < n) {
        uint64_t m = next_candidate;          /* candidate m = 4k+3 */
        int sum = 0;                          /* running partial sum */
        bool ok = true;

        uint64_t limit = (m - 1) / 2;          /* highest u to test */

        for (uint64_t u = 1; u <= limit; ++u) {
            int j = jacobi_symbol(u, m);
            sum += j;
            if (sum == 0) {                   /* zero partial sum found */
                ok = false;
                break;
            }
        }

        /* Store term if it satisfies the condition. */
        if (ok) {
            if (term_count == term_cap) {          /* grow cache if needed */
                term_cap <<= 1;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_count++] = (seq_t)m;
        }

        next_candidate += 4;                 /* advance to next 4k+3 */
    }
}

/* Main sequence function requested by the framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);          /* make sure we have at least n terms */
    return terms[n - 1];      /* n is 1‑based in OEIS */
}
