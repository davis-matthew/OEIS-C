/*
 * A008556  Triangle of coefficients of Legendre polynomials 2^n P_n (x).
 * https://oeis.org/A008556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the row containing the n‑th term, k is
 *            the offset within that row, and T(r,k) = C(2*(r-k), r-k) *
 *            C(r-k, k). The row index r is chosen such that the cumulative
 *            number of triangle entries up to row r-1 is ≤ n < those up to
 *            row r. Each row r has floor(r/2)+1 non‑zero entrie...
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 80, u64 n <= 258, u128 n <= 934,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* uses: seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative entry count up to
       row r-1 is <= n and up to row r is > n. */
    idx_t r = 0;
    idx_t cum = 0;

    while (1) {
        idx_t terms = r / 2 + 1;            /* number of non‑zero entries in row r */
        if (cum + terms > n)
            break;
        cum += terms;
        r++;
    }

    /* offset within the found row gives the coefficient index k */
    idx_t k = n - cum;                      /* 0 <= k <= r/2 */
    idx_t n_minus_k = r - k;                /* common part of the binomials */

    /* Compute T(r,k) = C(2*(r-k), r-k) * C(r-k, k) */
    seq_t part1 = binomial(2 * n_minus_k, n_minus_k);
    seq_t part2 = binomial(n_minus_k, k);

    return part1 * part2;
}
