/*
 * A109450  Triangle T(n,k), 0<=k<=n, read by rows, given by [0, 1, 0, 0, 0,
 *          0, 0, 0, 0, 0, ...] DELTA [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, ...]
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A109450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flatten(T(r,k)) where r = the largest integer with
 *            r(r+1)/2 ≤ n, k = n – r(r+1)/2. T(r,0)=0 for r>0,
 *            T(r,r)=Catalan(r), and for 0<k<r T(r,k) =
 *            (3r-3k+2)*binom(3r-k-1,k-1)/(3r-2k+1)
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom_nk, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_nk.h"
#include "../utils/catalan.h"

/* Compute the row index r corresponding to the flattened index n. */
static inline idx_t row_of_index(idx_t n)
{
    idx_t r = 0;
    /* Increment r while the next row would still be within n. */
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

/* Compute T(r,k) according to the definition above. */
static inline seq_t triangle_T(idx_t r, idx_t k)
{
    if (r == 0)             /* T(0,0) = 1 */
        return (seq_t)1;
    if (k == 0)             /* T(r,0) = 0 for r>0 */
        return (seq_t)0;
    if (k == r)             /* T(r,r) = Catalan(r) */
        return catalan(r);

    /* 0 < k < r */
    seq_t coeff   = (seq_t)(3 * r - 3 * k + 2);
    seq_t bin     = binom_nk(3 * r - k - 1, k - 1);
    seq_t numerator   = coeff * bin;
    seq_t denominator = (seq_t)(3 * r - 2 * k + 1);
    return numerator / denominator;
}

/* Function required by the build. */
seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t k = n - r * (r + 1) / 2;
    return triangle_T(r, k);
}
