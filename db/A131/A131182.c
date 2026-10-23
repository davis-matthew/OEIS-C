/*
 * A131182  Table T(n,k) = n!*k^n, read by upwards antidiagonals.
 * https://oeis.org/A131182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (d - p)! * p^(d - p) * where d is the largest integer
 *            with d*(d+1)/2 ≤ n, * and p = n - d*(d+1)/2. This corresponds to
 *            the * position (n,k) in the table T(n,k) = n! * k^n * read by
 *            upwards antidiagonals (n decreasing, k increasing)
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t tri(idx_t d)
{
    return (idx_t)(((unsigned __int128)d * (d + 1)) / 2);
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t lo = 0, hi = 1;
    while (tri(hi) <= n) hi <<= 1;          /* exponential search */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (tri(mid) <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t d = lo;
    idx_t before = tri(d);
    idx_t pos = n - before;                 /* position in diagonal d */
    idx_t nn = d - pos;                     /* n in T(n,k) */
    idx_t kk = pos;                         /* k in T(n,k) */

    /* Evaluate n! * k^n */
    if (nn == 0) return (seq_t)1;           /* 0! * k^0 = 1 */

    if (kk == 0) return (seq_t)0;           /* n>0 and k==0 => 0 */

    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= nn; ++i)
        fact *= (seq_t)i;

    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < nn; ++i)
        power *= (seq_t)kk;

    return fact * power;
}
