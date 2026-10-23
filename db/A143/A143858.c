/*
 * A143858  Number of pairwise disjoint unions of m integer-to-integer
 *          subintervals of [0,n]; a rectangular array by antidiagonals,
 *          n>=2m-1, m>=1.
 * https://oeis.org/A143858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = C(col+1, 2*m) where d is the smallest integer with
 *            d(d+1)/2 ≥ n, col = m + d - 1, m = 1 + (n - T_{d-1} - 1),
 *            T_{d-1} = (d-1)d/2. This implements the antidiagonal enumeration
 *            of the array R(m,n) = C(n+1,2m)
 * Range:     u8 n <= 30, u16 n <= 95, u32 n <= 333, u64 n <= 1195,
 *            u128 n <= 4312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline seq_t binom_value(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    for (idx_t i = 1; i <= k; ++i)
    {
        __int128 t = (__int128)res * (n - k + i);
        t /= i;
        res = (seq_t)t;
    }
#else
    for (idx_t i = 1; i <= k; ++i)
    {
        res = (res * (n - k + i)) / i;
    }
#endif
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* sequence starts at offset 1 */

    /* Find the diagonal index d so that T_{d-1} < n ≤ T_d */
    idx_t d = 1;
    idx_t prev_tri = 0;
    while (prev_tri + d < n) {
        prev_tri += d;
        ++d;
    }

    /* Position inside the block */
    idx_t r   = n - prev_tri - 1;    /* 0‑based offset in the current block */
    idx_t m   = r + 1;               /* m ranges 1 … d */
    idx_t col = m + d - 1;           /* column index in the array */

    return binom_value(col + 1, 2 * m);
}
