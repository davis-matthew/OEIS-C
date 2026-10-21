/*
 * A213205  T(n,k) = ((k+n)^2-4*k+3+(-1)^k-2*(-1)^n-(k+n)*(-1)^(k+n))/2; n , k
 *          > 0, read by antidiagonals.
 * https://oeis.org/A213205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the values of T(i,j) (i,j >= 1) along *
 *            antidiagonals, where * * T(i,j) = ((i+j)^2 - 4*j + 3 + (-1)^j -
 *            2*(-1)^i - (i+j)*(-1)^(i+j)) / 2 * * i.e. a(1)=T(1,1),
 *            a(2)=T(1,2), a(3)=T(2,1), a(4)=T(1,3),
 * Range:     u8 n <= 243, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   antidiag_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiag_start.h"

/* find t such that antidiag_start(t) <= n < antidiag_start(t+1) */
static inline idx_t find_t(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* find upper bound */
    while (antidiag_start(high) <= n) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* binary search for largest t with start <= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (antidiag_start(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;          /* largest t satisfying start(t) <= n */
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* the sequence starts at index 1 */
    if (n == 0)
        return 0;           /* undefined, but return 0 for safety */

    /* locate the antidiagonal t */
    idx_t t = find_t(n);

    /* compute row (i) and column (j) (both 1‑based) */
    idx_t i = n - ( ( ( (__int128) t * (t + 1) ) / 2 ) );
    idx_t j = ( ( ( (__int128) t * t + (__int128) 3 * t + 4 ) / 2 ) ) - n;

    /* compute T(i,j) */
    __int128 ii = ( __int128 )i;
    __int128 jj = ( __int128 )j;
    __int128 sum = 0;

    /* parity signs */
    int sign_j = (j % 2 == 0) ? 1 : -1;
    int sign_i = (i % 2 == 0) ? 1 : -1;
    int sign_ij = (((i + j) % 2) == 0) ? 1 : -1;

    /* (i+j)^2 */
    sum += (ii + jj) * (ii + jj);

    /* -4*j */
    sum -= 4 * jj;

    /* +3 */
    sum += 3;

    /* +(-1)^j */
    sum += sign_j;

    /* -2*(-1)^i */
    sum -= 2 * sign_i;

    /* -(i+j)*(-1)^(i+j) */
    sum -= (ii + jj) * sign_ij;

    /* divide by 2 */
    sum /= 2;

    return (seq_t)sum;
}
