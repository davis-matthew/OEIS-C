/*
 * A216249  T(n,k) = ((n+k)^2-4*k+3-2*(-1)^n+(-1)^k-(n+k-4)*(-1)^(n+k))/2-2,
 *          if k=1 and (n mod 2)=1;
 *          T(n,k)=((n+k)^2-4*k+3-2*(-1)^n+(-1)^k-(n+k-4)*(-1)^(n+k))/2, else.
 *          Table T(n,k) read by antidiagonals; n , k > 0.
 * https://oeis.org/A216249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,j) =
 *            ((i+j)^2-4*j+3-2*(-1)^i+(-1)^j-(i+j-4)*(-1)^(i+j))/2 with an
 *            extra -2 when j==1 and i is odd. The sequence is obtained by
 *            reading the table T along antidiagonals
 * Range:     u8 n <= 243, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* out of range, harmless */

    /*--- find the diagonal s = i + j (starting from 2) that contains a(n) ---*/
    idx_t low = 1, high = n + 2;                /* high is guaranteed large enough */
    idx_t s;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* cumulative terms up to diagonal mid: (mid-1)*mid/2 */
        __int128 tri = (__int128)(mid - 1) * mid / 2;
        if (tri >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    s = low;

    /*--- compute pair (i,j) for position n on that diagonal ---*/
    __int128 before = (__int128)(s - 2) * (s - 1) / 2;   /* terms before this diagonal */
    idx_t offset = n - (idx_t)before;                   /* 1‑based offset on the diagonal */
    idx_t i = offset;                                 /* row index (n)    */
    idx_t j = s - offset;                              /* column index (k) */

    /*--- compute T(i,j) ---*/
    __int128 sum   = (__int128)i + j;                  /* i + j        */
    __int128 base  = sum * sum;                        /* (i+j)^2     */
    base -= (__int128)4 * j;                           /* -4*j         */
    base += 3;                                         /* +3          */
    int sign_i   = (i & 1) ? -1 : 1;                   /* (-1)^i        */
    int sign_j   = (j & 1) ? -1 : 1;                   /* (-1)^j        */
    int sign_sum = (sum & 1) ? -1 : 1;                 /* (-1)^(i+j)    */
    base += (-2 * sign_i);                             /* -2*(-1)^i     */
    base += sign_j;                                    /* +(-1)^j       */
    base += -((sum - 4) * sign_sum);                   /* -(i+j-4)*(-1)^(i+j) */

    seq_t val = (seq_t)(base / 2);                     /* division by 2 is exact */
    if (j == 1 && (i & 1))                            /* special case          */
        val -= (seq_t)2;

    return val;
}
