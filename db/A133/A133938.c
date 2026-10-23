/*
 * A133938  Triangle read by rows: A007318 * (A129686 + A133080 - I) as
 *          infinite lower triangular matrices, where I is the identity
 *          matrix.
 * https://oeis.org/A133938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 159, u32 n <= 607, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle defined by T(r,k) = C(r,k) + ((1+(-1)^k)/2)*C(r,k+1) + C(r,k+2).
   a(n) is obtained by reading the entries row‑wise. */
#include <stdint.h>
#include "../utils/binomial.h"

static idx_t isqrt_u64(idx_t x) {
    idx_t low = 0;
    idx_t high = (x >> 1) + 1;      /* high = floor(x/2)+1  is big enough      */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq == x)            return mid;
        if (sq < x)             low = mid + 1;
        else                    high = mid - 1;
    }
    return high;                 /* high is floor(sqrt(x))                          */
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t val = 8 * n + 1;                     /* 8*n+1 fits in idx_t for expected n               */
    idx_t s   = isqrt_u64(val);                /* floor(sqrt(8*n+1))                                */
    idx_t r   = (s - 1) / 2;                   /* row index                                       */
    idx_t row_start = r * (r + 1) / 2;          /* first index of this row                         */
    idx_t k = n - row_start;                   /* column index within the row                     */

    seq_t res = binomial(r, k);                /* C(r,k)                                            */
    if ((k & 1) == 0)                          /* add C(r,k+1) if k is even                         */
        res += binomial(r, k + 1);
    res += binomial(r, k + 2);                 /* add C(r,k+2)                                       */

    return res;
}
