/*
 * A103883  Square array A(n,k) read by antidiagonals: coordination sequence
 *          for lattice B_n.
 * https://oeis.org/A103883
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array * A(n,k) (defined
 *            by A(n,k) = Σ_{j=0..k} binom(n+k-j-1,n-1) * * (binom(2*n+1,2*j)
 *            - 2*j*binom(n,j))) by antidiagonals, * starting with n ≥ 2. The
 *            argument n is the sequence index (a(2) * is the first term). The
 *            function works for any n ≥ 2 and * uses t...
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 198, u64 n <= 719,
 *            u128 n <= 1226, big any n
 * Validated: standard, pass, 71 terms through n = 1162 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Offset of the sequence starts at 2 */
    if (n < 2) return 0;

    idx_t idx = n;                 /* sequence index (≥2) */
    idx_t val = idx - 1;           /* 1‑based count of elements from the first term */

    /* Find the smallest s such that (s-1)*s/2 ≥ val.
       This computes the diagonal (nRow + k) in the array.  */
    idx_t s = 2;
    while (1) {
        unsigned __int128 t = (unsigned __int128)(s - 1) * s / 2;
        if (t >= (unsigned __int128)val) break;
        ++s;
    }

    /* Compute previous cumulative term count and offset within diagonal */
    unsigned __int128 prev128 = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t prev = (idx_t)prev128;
    idx_t offset = val - prev - 1;          /* 0‑based position in the diagonal */
    idx_t nRow = s - offset;               /* row index of the array element   */
    idx_t k    = offset;                   /* column index of the array element */

    /* Compute A(nRow,k) from the given formula */
    seq_t result = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t part1 = binomial(2 * nRow + 1, 2 * j);
        seq_t part2 = binomial(nRow, j);
        seq_t term  = part1 - ((seq_t)2 * (seq_t)j * part2);
        seq_t coeff = binomial(nRow + k - j - 1, nRow - 1);
        result += coeff * term;
    }
    return result;
}
