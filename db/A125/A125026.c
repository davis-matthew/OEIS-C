/*
 * A125026  Triangle read by rows: T(n,k) = k*binomial(n,k) + binomial(n-1,k)
 *          (1 <= k <= n).
 * https://oeis.org/A125026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row(n),col(n)) where T(n,k) = k*binomial(n,k) +
 *            binomial(n-1,k) and row(n) is the least r such that r*(r+1)/2 ≥
 *            n, col(n) = n – r*(r-1)/2
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binom, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* compute the row number r */
    idx_t s = int_sqrt(8 * n + 1);      /* floor(sqrt(8n+1)) */
    idx_t r = (s - 1) / 2;              /* floor((sqrt(8n+1)-1)/2) */
    if (r * (r + 1) / 2 < n)
        r++;                            /* adjust if needed */

    /* column index k (1‑based) */
    idx_t k = n - (r * (r - 1) / 2);

    /* compute binomial(r,k) */
    seq_t bin1 = binom(r, k);

    /* term = k * binom(r,k) + binom(r-1,k) */
    seq_t term = (seq_t)k * bin1;

    if (k < r) {
        /* binom(r-1,k) = binom(r,k) * (r-k) / r */
        seq_t bin2 = bin1 * (seq_t)(r - k) / (seq_t)r;
        term += bin2;
    }

    return term;
}
