/*
 * A228312  Triangle read by rows: T(m,n) (1<=n<=m) is the hyper-Wiener index
 *          of the complete bipartite graph K(m,n).
 * https://oeis.org/A228312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the triangle read by rows of T(m,k) = (3(m+k)^2 - 4mk -
 *            3m - 3k) / 2, where the index n is mapped to the unique pair
 *            (m,k) with 1 ≤ k ≤ m and t(m‑1) < n ≤ t(m) (t(m) = m(m+1)/2)
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for n=0 */

    /* Find the row m: smallest m such that m*(m+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;                     /* upper bound: t(n) >= n */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t tmid = ((seq_t)mid * ((seq_t)mid + 1)) / 2;
        if (tmid >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Compute t(m-1) = (m-1)*m/2 */
    idx_t prev_m = m - 1;
    seq_t t_prev = ((seq_t)prev_m * (seq_t)m) / 2;

    /* Column index k */
    idx_t k = (idx_t)((seq_t)n - t_prev);    /* 1‑based */

    /* Evaluate the hyper‑Wiener index formula */
    seq_t m_seq = (seq_t)m;
    seq_t k_seq = (seq_t)k;
    seq_t sum   = m_seq + k_seq;             /* m + k */
    seq_t res = ( ((seq_t)3 * sum * sum)          /* 3(m+k)^2   */
                 - ((seq_t)4 * m_seq * k_seq)     /* -4mk       */
                 - ((seq_t)3 * m_seq)            /* -3m        */
                 - ((seq_t)3 * k_seq) ) / 2;     /* -3k, /2    */

    return res;
}
