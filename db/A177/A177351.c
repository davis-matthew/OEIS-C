/*
 * A177351  Triangle t(n,k)= sum_{m=1..floor(n/2-k)} binomial(n-m-k,m+k),
 *          -floor(n/2) <= k <= floor(n/2), read by rows.
 * https://oeis.org/A177351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = t(r,k) where - r is the row with index n: C(r-1) ≤ n <
 *            C(r) - k = -⌊r/2⌋ + (n - C(r-1)) - t(r,k) = Σ_{m =
 *            max(1,−k)}^{⌊r/2⌋ − k} binomial(r − m − k , m + k) C(r) =
 *            r(r+1)/2 + ⌊r/2⌋ + 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"          /* provides seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find the row r containing the linear index n. */
    idx_t r = 0;
    while (1) {
        __int128 tri = (__int128)r * (r + 1) / 2;          /* r(r+1)/2   */
        idx_t   C   = (idx_t)tri + (r >> 1) + 1;             /* C(r) value */
        if (C > n) break;
        r++;
    }

    /* C(r-1) is the first index in row r. */
    idx_t prevC;
    if (r == 0) {
        prevC = 0;
    } else {
        idx_t prev = r - 1;
        __int128 tri = (__int128)prev * (prev + 1) / 2;
        prevC = (idx_t)tri + (prev >> 1) + 1;
    }

    idx_t offset = n - prevC;                 /* position inside row r   */
    idx_t half   = r >> 1;                    /* ⌊r/2⌋                    */
    int64_t k    = -(int64_t)half + (int64_t)offset; /* signed k                    */

    /* Determine the range of m. */
    int64_t m_start = 1;
    if (k < 0) m_start = -k;                 /* ensures m + k ≥ 0         */
    int64_t m_max   = (int64_t)half - k;      /* ⌊r/2⌋ – k                  */

    if (m_max < 1 || m_start > m_max) return 0;

    seq_t val = 0;

    for (int64_t m = m_start; m <= m_max; ++m) {
        idx_t n_bin, k_bin;
        if (k < 0) {
            n_bin = r - (idx_t)m + (idx_t)(-k);
        } else {
            n_bin = r - (idx_t)m - (idx_t)k;
        }
        k_bin = (idx_t)(m + k);               /* m + k ≥ 0                 */
        val += binomial(n_bin, k_bin);
    }

    return val;
}
