/*
 * A209490  Table T(d,n), read by antidiagonals, gives the number of subsets
 *          of length n containing an arithmetic progression of length 3 with
 *          distance d.
 * https://oeis.org/A209490
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(d,n0) read by antidiagonals from Table T(d,n) where
 *            T(d,n)=2^n - prod_{i=0}^{d-1} Tri(floor((n+i)/d)+2) (Tri(0)=0,
 *            Tri(1)=1, Tri(2)=1)
 * Range:     u8 n <= 22, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   compute_tribonacci, index_to_dn, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- auxiliary utilities ------------------------------------------------- */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
/* seq_t and idx_t are defined by the build. */
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */
#include "../utils/compute_tribonacci.h"
#include "../utils/index_to_dn.h"

/* ---- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)3) return (seq_t)0;   /* behaviour below offset undefined */

    idx_t d, n0;
    index_to_dn(n, &d, &n0);

    /* Maximum floor((n0 + i)/d) needed in the product */
    idx_t max_floor = (n0 + d - (idx_t)1) / d;
    size_t trib_needed = (size_t)(max_floor + (idx_t)2);   /* indices up to this */

    /* Build small tribonacci table */
    seq_t *trib = (seq_t*)malloc((trib_needed + 1) * sizeof(seq_t));
    if (!trib) abort();   /* allocation failure */
    compute_tribonacci(trib, trib_needed);

    /* Compute product of tribonacci values */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < d; ++i) {
        idx_t floor_val = (n0 + i) / d;
        prod *= trib[(size_t)floor_val + 2];
    }

    free(trib);

    seq_t pow2_val = pow2(n0);
    if (prod > pow2_val) return (seq_t)0;   /* safety, should not happen */
    return pow2_val - prod;
}
