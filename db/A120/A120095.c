/*
 * A120095  Triangle T(n,k) = total of number at last index for all set
 *          partitions of n into k parts.
 * https://oeis.org/A120095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = ⌈(√(8n+1)-1)/2⌉ , c = n - r(r-1)/2 * *
 *            T(n,k) = (k(k+1)/2) * S2(n-1,k) + k * S2(n-1,k-1) * with S2(m,k)
 *            the Stirling numbers of the second kind. * * The algorithm
 *            builds Stirling numbers for row r-1 by a two‑row DP * and
 *            computes the required T(r,c). Complexity is O(r²)...
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 125, u64 n <= 332,
 *            u128 n <= 915, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* External linkage */
seq_t a(idx_t n)
{
    /* ----- find the row r (1‑based) in which a(n) lies ----- */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while (r * (r + 1) / 2 < n)
        ++r;

    /* ----- column within that row ----- */
    idx_t c = n - (r - 1) * r / 2;                 /* 1 ≤ c ≤ r */

    /* trivial case */
    if (n == 1)
        return (seq_t)1;

    /* ----- allocate arrays for one row of Stirling numbers ----- */
    size_t m = (size_t)r;                         /* indices 0 … r-1 */
    seq_t *prev = alloc_seq_array(m);
    seq_t *curr = alloc_seq_array(m);
    if (!prev || !curr) {                         /* allocation failure */
        free_seq_array(prev);
        free_seq_array(curr);
        return (seq_t)0;
    }

    /* ----- initialise previous row: S2(0,0)=1 ----- */
    for (size_t i = 0; i < m; ++i)
        prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    /* ----- compute rows 1 … r-1 of S2 ----- */
    for (idx_t i = 1; i < r; ++i) {
        /* curr[0] = 0 automatically by zeroing the part below */
        curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            curr[k] = (seq_t)k * prev[k] + prev[k - 1];
        }
        /* zero unused trailing elements */
        for (idx_t k = i + 1; k < m; ++k)
            curr[k] = (seq_t)0;

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* ----- fetch the two Stirling numbers needed ----- */
    seq_t Sk   = (c <= r - 1) ? prev[(size_t)c] : (seq_t)0;
    seq_t Skm1 = (c > 0 && c - 1 <= r - 1) ? prev[(size_t)(c - 1)] : (seq_t)0;

    /* ----- compute T(r,c) ----- */
    seq_t tri    = ((seq_t)c * (seq_t)(c + 1)) / 2;
    seq_t result = tri * Sk + (seq_t)c * Skm1;

    /* ----- clean up ----- */
    free_seq_array(prev);
    free_seq_array(curr);

    return result;
}
