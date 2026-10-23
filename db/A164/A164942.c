/*
 * A164942  Triangle, read by rows, T(n,k) = (-1)^k*binomial(n, k)*3^(n-k).
 * https://oeis.org/A164942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^k * binomial(r,k) * pow3(r-k), where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     i8 n <= 14, i16 n <= 45, i32 n <= 155, i64 n <= 567,
 *            i128 n <= 2221, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if ((idx_t)t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    __int128 offset128 = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)offset128;

    seq_t val = binom(r, k);            /* binomial coefficient */
    seq_t p   = pow3(r - k);            /* 3^(r-k) */
    val = val * p;                      /* product */

    if (k & 1)                          /* apply (-1)^k */
        val = -val;

    return val;
}
