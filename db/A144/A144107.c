/*
 * A144107  Eigentriangle, row sums = n!
 * https://oeis.org/A144107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Eigentriangle (OEIS A144107) – read row by row.
   The triangle T is defined by
        T(r,k) = A003319(r-k+1) * (k-1)!   , 1 ≤ k ≤ r
   with A003319(1)=1 and
        A003319(n) = n! – Σ_{j=1}^{n-1} A003319(j) * (n-j)! .
   The sequence a(n) is the n-th entry of the triangle read row by row.
   The implementation recomputes the required row on each call,
   thus is correct for arbitrary order of queries. */

/* Includes needed for dynamic allocation and array management. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array(size_t)   */
#include "../utils/seq_free_array.h"    /* seq_free_array(seq_t*)    */

/* Triangular number, used only for index calculation. */
static inline idx_t triangular_idx(idx_t t) { return t*(t+1)/2; }

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; a(1) is the first term */
        return (seq_t)0;

    /* Find the row r such that triangular(r-1) < n ≤ triangular(r). */
    idx_t low  = 1, high = 1;
    while (triangular_idx(high) < n) high <<= 1;          /* exponential search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular_idx(mid) >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t r = low;

    idx_t prev = triangular_idx(r - 1);   /* elements before this row */
    idx_t k = n - prev;                   /* column index in this row (1‑based) */

    /* Allocate temporary arrays for factorials and A003319 up to r. */
    seq_t *fact = seq_alloc_array((size_t)r + 1);
    seq_t *a003 = seq_alloc_array((size_t)r + 1);

    /* factorial[0] = 0! = 1 */
    fact[0] = (seq_t)1;
    /* Compute factorials 1! … r! */
    for (idx_t i = 1; i <= r; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    /* Compute A003319(1)…A003319(r) */
    a003[1] = (seq_t)1;                   /* A003319(1) = 1 */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < i; ++j) {
            seq_t prod = a003[j] * fact[i-j];
            sum += prod;
        }
        a003[i] = fact[i] - sum;
    }

    /* Value at position T(r,k).  k ≥ 1, thus k-1 ≤ r-1. */
    seq_t result = a003[r - k + 1] * fact[k - 1];

    /* Free temporary arrays */
    seq_free_array(fact);
    seq_free_array(a003);

    return result;
}
