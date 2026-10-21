/*
 * A203953  Symmetric matrix based on (1,2,1,2,1,2,...), by antidiagonals.
 * https://oeis.org/A203953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry read along antidiagonals of M = T' * T * where
 *            T is constructed from the periodic sequence (1,2,1,2,…) . * The
 *            formula is derived by summing s[p‑k+1]·s[q‑k+1] over
 *            k=1..min(p,q) * with s[j] = 2 – (j % 2). Using parity simplifies
 *            the sum to a * constant number of cases. The algor...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the smallest s such that (s-1)*s/2 >= n.
       Binary search – safe for very large n. */
    uint64_t lo = 1, hi = 2;
    while (((__int128)(hi - 1) * hi) / 2 < n) hi <<= 1;
    while (lo < hi)
    {
        uint64_t mid = lo + (hi - lo) / 2;
        if (((__int128)(mid - 1) * mid) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t s = lo;                      /* sum index (row+col) */

    /* Number of elements before this diagonal */
    uint64_t prev_tri = ((__int128)(s - 2) * (s - 1)) / 2;
    uint64_t idx = n - prev_tri - 1;      /* 0‑based index within diagonal */

    uint64_t p = idx + 1;                 /* row number   (>=1) */
    uint64_t q = s - idx - 1;             /* column number(>=1) */

    uint64_t m = p < q ? p : q;           /* min(p,q)     */

    /* Parities of (p+1) and (q+1) */
    uint64_t a_par = (p + 1) & 1;
    uint64_t b_par = (q + 1) & 1;

    /* Counts of odd/even k in 1..m */
    uint64_t odd_cnt  = (m + 1) >> 1;      /* ceil(m/2)    */
    uint64_t even_cnt = m >> 1;           /* floor(m/2)   */

    /* Compute product for odd k (e1,f1) and even k (e0,f0) */
    uint64_t e0 = a_par;        /* (a+0)%2 */
    uint64_t f0 = b_par;        /* (b+0)%2 */
    uint64_t e1 = 1 - e0;       /* (a+1)%2 */
    uint64_t f1 = 1 - f0;       /* (b+1)%2 */

    /* s[x] = 2 - (x%2) */
    uint64_t prod_even = (2 - e0) * (2 - f0); /* for even k */
    uint64_t prod_odd  = (2 - e1) * (2 - f1); /* for odd  k */

    /* Sum contributions */
    uint64_t sum = prod_even * even_cnt + prod_odd * odd_cnt;

    return (seq_t)sum;
}
