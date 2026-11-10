/*
 * A050161  T(n,k)=S(2n+1,n-1,k-1), 0<=k<=n, n >= 0, array S as in A050157.
 * https://oeis.org/A050161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = C(2n+1,n-1) - C(2n+1,n-1-k) for 1<=k<=n (second term 0
 *            when k=n); T(n,0) = Catalan(n+1) - Catalan(n) (A000245); read by
 *            rows, n>=0, 0<=k<=n
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-07)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t N)
{
    /* row n: largest n with n(n+1)/2 <= N */
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= N)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1ULL) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= N)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t n = lo;
    idx_t k = N - (idx_t)((unsigned __int128)n * (n + 1) / 2);

    if (k == 0)
        return catalan(n + 1) - catalan(n);

    /* here n >= 1 */
    seq_t top = binomial(2 * n + 1, n - 1);
    if (k >= n)
        return top;
    return top - binomial(2 * n + 1, n - 1 - k);
}
