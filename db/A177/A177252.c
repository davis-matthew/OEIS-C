/*
 * A177252  Triangle read by rows: T(n,k) is the number of permutations of [n]
 *          having k adjacent 4-cycles (0 <= k <= floor(n/4)), i.e., having k
 *          cycles of the form (i, i+1, i+2, i+3).
 * https://oeis.org/A177252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r and k are obtained from the row/column of
 *            * the n-th entry in the triangle T(n,k). T(r,k) is computed by *
 *            T(r,k) = Σ_{j=k..⌊r/4⌋} (-1)^{k+j} * binom(j,k) * (r-3j)! / j!.
 *            * See OEIS A177252
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 65, u128 n <= 170,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Evaluate the term T(r,k) of the triangle using the explicit formula. */
static seq_t compute_T(idx_t r, idx_t k)
{
    idx_t maxj = r >> 2;                 /* floor(r/4) */
    seq_t result = 0;

    for (idx_t j = k; j <= maxj; ++j)
    {
        /* C(j,k) */
        seq_t bin = binom(j, k);

        /* (r-3j)! / j!  = ∏_{t=j+1}^{r-3j} t */
        seq_t comb = 1;
        idx_t upper = r - 3 * j;
        for (idx_t t = j + 1; t <= upper; ++t)
            comb *= (seq_t)t;

        seq_t term = bin * comb;
        if ( ((k + j) & 1) == 0 )
            result += term;   /* even power of -1 */
        else
            result -= term;   /* odd power of -1 */
    }
    return result;
}

/* Main sequence entry function */
seq_t a(idx_t n)
{
    /* Find the row r and column k such that the n-th entry lies in row r
       and column k of the triangle.  Offsets are 0‑based. */
    idx_t r = 0;
    idx_t entries_seen = 0;
    while (1) {
        idx_t row_len = (r >> 2) + 1;   /* ⌊r/4⌋ + 1 */
        if (n < entries_seen + row_len)
            break;
        entries_seen += row_len;
        ++r;
    }
    idx_t k = n - entries_seen;        /* column within the row */
    return compute_T(r, k);
}
