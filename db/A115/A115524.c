/*
 * A115524  Number triangle (1,-x)+(x,x)/2+(x,-x)/2-(x^2,x^2) (expressed using
 *          the notation of stretched Riordan arrays).
 * https://oeis.org/A115524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = value of the n-th element in the flattened triangle
 *            defined by: T(n,k) = (-1)^n * ( 1 if n==k -1 if n==2k+2
 *            -(1+(-1)^k)/2 if n==k+1 ) where all other entries are 0. The
 *            flattening is row-wise: first row n=0, then row n=1, then n=2, …
 * Range:     i8 n <= 9038, i16 n <= 9038, i32 n <= 9038, i64 n <= 9038,
 *            i128 n <= 9038, big any n
 * Helpers:   parity_sign
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity_sign.h"

seq_t a(idx_t n) {
    /* binary search for the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = (1ULL << 32);   /* r <= ~4.3e9 for n < 2^64 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    unsigned __int128 prodR = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)prodR;  /* column within row r, 0 <= c <= r */

    int val = 0;

    if (c == r) {
        /* case n == k */
        val = parity_sign(r);
    } else if (r >= 2 && ((r & 1) == 0) && c == (r / 2) - 1) {
        /* case n == 2k+2 => k = (n-2)/2 = r/2 -1, r even */
        val = -parity_sign(r); /* (-1)^(r+1) */
    } else if (c == r - 1) {
        /* case n == k+1 => k = c */
        if ((c & 1) == 0) {
            /* -(1+(-1)^k)/2 == -1 when k even */
            val = -parity_sign(r); /* (-1)^(r+1) */
        } else {
            /* term is 0 when k odd */
            val = 0;
        }
    } else {
        val = 0;
    }

    return (seq_t)val;
}
