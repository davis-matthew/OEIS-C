/*
 * A048472  Array T by antidiagonals, T(k,n)=(k+1)*n*2^(n-1)+1, n >= 0, k >=
 *          1.
 * https://oeis.org/A048472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(k,m) where k+m = d, * d = floor((sqrt(8*n+1)-1)/2), * k
 *            = n - d(d+1)/2, * m = d - k, * and T(k,m) = (k+1)*m*2^(m-1)+1
 *            (with T(k,0)=1)
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline seq_t compute_T(idx_t k, idx_t m)
{
    if (m == 0) return 1;                /* (k+1)*0*2^(–1) + 1 = 1 */
    seq_t mult   = ((seq_t)(k + 1)) * (seq_t)m;   /* (k+1) * m */
    seq_t pow2   = 1;
    /* compute 2^(m-1) safely; for m large this may overflow, which is fine */
    for (idx_t i = 1; i < m; ++i) pow2 *= ((seq_t)2);
    return mult * pow2 + 1;
}

seq_t a(idx_t n)
{
    /* find the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = 0;
    __int128 cumulative = 0;          /* sum of terms in previous diagonals */
    while (cumulative + d + 1 <= n)  /* cumulative holds sum up to diag d-1 */
    {
        cumulative += d + 1;
        ++d;
    }
    /* offset inside diagonal d */
    idx_t offset = n - (idx_t)cumulative;
    idx_t k = offset;                 /* column index on the diagonal */
    idx_t m = d - k;                  /* row index on the diagonal */
    return compute_T(k, m);
}
