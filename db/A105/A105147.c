/*
 * A105147  Triangular array read by rows: T(n,k) = number of compositions of
 *          n having smallest part equal to k.
 * https://oeis.org/A105147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = number of compositions of r having smallest part k, where
 *            r and k are derived from n as follows: let r be the unique
 *            integer with r(r-1)/2 < n ≤ r(r+1)/2 (row number), let k = n –
 *            (r-1)r/2 (position within the row). Then T(r,k) =
 *            Σ_{t=1}^{⌊r/k⌋} binom(r – t(k-1) – 1, t-1) – Σ_{t=1}^{⌊r/(k+...
 * Range:     u8 n <= 36, u16 n <= 153, u32 n <= 561, u64 n <= 2145,
 *            u128 n <= 8385, big any n
 * Helpers:   binom, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/row_of.h"

static seq_t compute_f(idx_t n, idx_t k)
{
    idx_t max_r = n / k;
    seq_t sum = 0;
    for (idx_t r = 1; r <= max_r; ++r)
    {
        idx_t N = n - r * (k - 1) - 1;   /* N ≥ 0 */
        idx_t K = r - 1;
        sum += binom(N, K);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n=0 is undefined */
        return 0;

    idx_t r = row_of(n);                /* r such that r(r+1)/2 ≤ n */
    if (r * (r + 1) / 2 < n)             /* adjust for n > T(r) */
        ++r;

    idx_t k = n - (r - 1) * r / 2;       /* position in row r */

    if (k == r)                          /* last element of the row */
        return 1;

    seq_t f_k  = compute_f(r, k);
    seq_t f_k1 = compute_f(r, k + 1);
    return f_k - f_k1;
}
