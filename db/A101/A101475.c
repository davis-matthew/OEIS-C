/*
 * A101475  Triangle T(n,k) read by rows: number of lattice paths from (0,0)
 *          to (0,2n) with steps (1,1) or (1,-1) that stay between the lines
 *          y=0 and y=k.
 * https://oeis.org/A101475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, walk
 * Formula:   a(n) = T(m,k) where m = largest integer with m(m+1)/2 <= n and k
 *            = n - m(m+1)/2. T(m,k) = sum_{i>=0} (C(2m, m-i*(k+2)) - C(2m,
 *            m+i*(k+2)+k+1)) (from OEIS A101475)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binom_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_big.h"

static seq_t triangle_T(idx_t m, idx_t k)
{
    seq_t total = 0;
    idx_t N = 2 * m;          /* horizontal size of binomial */
    idx_t step = k + 2;       /* i*(k+2) increment */

    for (idx_t i = 0;; ++i) {
        /* n1 = m - i*step; stop when n1 becomes negative */
        if (i * step > m) break;
        idx_t n1 = m - i * step;
        seq_t term1 = binom_big((seq_t)N, (seq_t)n1);

        /* n2 = m + i*step + k + 1 */
        idx_t n2 = m + i * step + k + 1;
        seq_t term2 = 0;
        if (n2 <= N)
            term2 = binom_big((seq_t)N, (seq_t)n2);

        /* If both terms are zero, further i will produce zeros */
        if (term1 == 0 && term2 == 0)
            break;

        total += term1;
        total -= term2;
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Find m such that m(m+1)/2 <= n < (m+1)(m+2)/2 */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t start = m * (m + 1) / 2;   /* first index of row m */
    idx_t k = n - start;            /* column in row m */

    return triangle_T(m, k);
}
