/*
 * A111420  a(n) = Sum_{q=0..n} Stirling2(n+1,q)^2*q!.
 * https://oeis.org/A111420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{q=0..n} Stirling2(n+1,q)^2 * q!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Offset: a(0) = 0 */
    if (n == 0) return 0;

    /* ---- Allocate temporary tables ------------------------------------- */
    /* factorials: fact[0..n] */
    size_t fsize = (size_t)n + 1;
    seq_t *fact = mem_new(seq_t, fsize);
    if (!fact) return 0; /* Allocation failed – return 0 as a fallback */

    /* Stirling numbers for the current and previous row:
       we need indices up to (n+1) */
    size_t rsize = (size_t)n + 2; /* rows 0..n+1 inclusive */
    seq_t *prev = mem_new(seq_t, rsize);
    seq_t *curr = mem_new(seq_t, rsize);
    if (!prev || !curr) { mem_free(fact); mem_free(prev); mem_free(curr); return 0; }

    /* ---- Compute factorials -------------------------------------------- */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[(size_t)i] = fact[(size_t)(i - 1)] * (seq_t)i;

    /* ---- Initialize Stirling row S(0,0)=1 ------------------------------ */
    prev[0] = 1;
    for (size_t k = 1; k < rsize; ++k)
        prev[k] = 0;

    /* ---- Build rows S(1,0)..S(n+1,n+1) --------------------------------*/
    for (idx_t m = 1; m <= n + 1; ++m) {
        curr[0] = 0; /* S(m,0)=0 for m>0 */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = prev[(size_t)(k - 1)];          /* S(m-1,k-1)   */
            seq_t term2 = prev[(size_t)k] * (seq_t)k;     /* k * S(m-1,k) */
            curr[(size_t)k] = term1 + term2;
        }
        /* Swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    /* After the loop, 'prev' holds S(n+1,k) for all k */

    /* ---- Compute the sum a(n) ---------------------------------------- */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t sq = prev[(size_t)k] * prev[(size_t)k]; /* S^2 */
        seq_t term = sq * fact[(size_t)k];            /* * k!   */
        sum += term;
    }

    /* ---- Clean up ----------------------------------------------------- */
    mem_free(fact);
    mem_free(prev);
    mem_free(curr);

    return sum;
}
