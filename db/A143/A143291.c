/*
 * A143291  Triangle T(n,k), n>=2, 0<=k<=n-2, read by rows: numbers of binary
 *          words of length n containing at least one subword 10^{k}1 and no
 *          subwords 10^{i}1 with i<k.
 * https://oeis.org/A143291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) are derived from n by locating the
 *            flattened triangle entry. T(n,k) = G(n,k+1) – G(n,k+2) G(n,d) =
 *            Σ_{m=0}^{⌊(n+d-1)/d⌋} binom(n - m·(d-1) + d - 1 , m) (binom is
 *            binomial coefficient)
 * Range:     u8 n <= 22, u16 n <= 121, u32 n <= 497, u64 n <= 2017,
 *            u128 n <= 8129, big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

/* number of binary words of length n whose consecutive '1's are
   separated by at least d zeros (G(n,d)) */
static seq_t compute_G(idx_t n, idx_t d)
{
    idx_t m_max = (n + d - 1) / d;          /* floor((n+d-1)/d) */
    seq_t total = 0;
    for (idx_t m = 0; m <= m_max; ++m)
    {
        idx_t arg = n - m * (d - 1) + d - 1;
        total += binom_n_k(arg, m);
    }
    return total;
}

/* a(idx_t n) returns the triangle entry at flattened index n.
   The triangle rows are:
     row 2 : 1
     row 3 : 3 1
     row 4 : 8 2 1
     …                                                              */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* offset starts at 2 */

    /* find the row r and column k corresponding to linear index n   */
    idx_t r = 2;                           /* current row number   */
    idx_t start = 2;                       /* starting index of row r */
    while (start + (r - 1) <= n)
    {
        start += r - 1;
        ++r;
    }
    idx_t k = n - start;                    /* column within the row */

    /* compute the value using the G() formula */
    seq_t v1 = compute_G(r, k + 1);
    seq_t v2 = compute_G(r, k + 2);
    return v1 - v2;
}
