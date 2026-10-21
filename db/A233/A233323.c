/*
 * A233323  Triangle read by rows: T(n,k) = number of palindromic compositions
 *          of n in which the largest part is equal to k, 1 <= k <= n.
 * https://oeis.org/A233323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 633, u32 n <= 2350, u64 n <= 8651,
 *            u128 n <= 9029, big any n
 * Helpers:   compute_comp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula and implementation:
 *  T(n,k) = number of palindromic compositions of n whose largest part is k.
 *
 *  For a given flattened index n, we first recover the row R and column k by
 *  inverting the triangular numbers.  Then we compute
 *
 *      comp_k[s]   = number of compositions of s with parts ≤ k
 *      comp_km1[s] = number of compositions of s with parts ≤ k-1
 *
 *  for all s ≤ floor(R/2).  These are obtained by a simple DP:
 *
 *      comp[0] = 1
 *      comp[s] = Σ_{i=1..min(k,s)} comp[s-i]
 *
 *  The desired T(n,k) is then
 *
 *      even part  (possible only if R even):
 *          comp_eq(R/2)  where   comp_eq[s] = comp_k[s] – comp_km1[s]
 *
 *      odd part:
 *          Σ_{c=1..k,  R−c even}
 *              if c == k      :  add comp_k[ (R−c)/2 ]
 *              else           :  add comp_eq[ (R−c)/2 ]
 *
 *  All arithmetic uses seq_t and idx_t as required.  The routine is
 *  fully functional for any seq_t (fixed‑size or big integer) and any
 *  idx_t (uint64_t).  No static cache is needed – each call builds only
 *  the small DP arrays of size ≤ ⌊R/2⌋+1, which is ≤ 5000 for R ≤ 10000.
 *  This algorithm runs in O(R·k) ≤ 5·10⁷ elementary operations, well
 *  within the time limit for the stated constraints.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_comp.h"

/* Compute the row R and column k (1‑based) from the flattened index n.
 * The function is independent of seq_t and works for uint64_t indices.   */
static void row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *row = low;
    seq_t prev_tri = (seq_t)(*row - 1) * (seq_t)*row / 2;
    *col = n - (idx_t)prev_tri;          /* columns are 1‑based */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined for n < 1 */

    /* Unpack flattened index into row R and column k */
    idx_t R, k;
    row_col(n, &R, &k);

    /* Maximum sum needed in the left half of a palindrome of n */
    idx_t maxS = R / 2;

    /* Allocate workspace for compositions with parts ≤ k and parts ≤ k-1 */
    seq_t *comp_k   = (seq_t *)calloc((size_t)maxS + 1, sizeof(seq_t));
    seq_t *comp_km1 = (seq_t *)calloc((size_t)maxS + 1, sizeof(seq_t));
    if (!comp_k || !comp_km1) {
        free(comp_k);
        free(comp_km1);
        return 0;          /* allocation failure – graceful return */
    }

    /* Compute the two composition tables */
    compute_comp(comp_k,   maxS, k);
    compute_comp(comp_km1, maxS, k - 1);

    seq_t result = 0;

    /* Even‑length palindromes (only if R is even) */
    if ((R & 1) == 0) {
        idx_t half = R / 2;
        seq_t comp_eq = comp_k[half] - comp_km1[half];
        result += comp_eq;
    }

    /* Odd‑length palindromes */
    for (idx_t c = 1; c <= k; ++c) {
        if (((R - c) & 1) != 0)            /* (R‑c) must be even */
            continue;
        idx_t rem = (R - c) >> 1;          /* (R‑c)/2 */
        seq_t add;
        if (c == k)
            add = comp_k[rem];             /* center part is k */
        else
            add = comp_k[rem] - comp_km1[rem];   /* left half must contain k */
        result += add;
    }

    free(comp_k);
    free(comp_km1);
    return result;
}
