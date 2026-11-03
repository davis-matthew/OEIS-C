/*
 * A038275  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*9^j.
 * https://oeis.org/A038275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) = binomial(i,j) * 7^(i-j) * 9^j
 * where i = floor((sqrt(8*n+1)-1)/2)  (row index)
 *       j = n - i*(i+1)/2              (position in that row)
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2  */
    idx_t low  = 0;
    idx_t high = n;
    idx_t i    = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            i = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t row_start = (idx_t)((__int128)i * (i + 1) / 2);
    idx_t j = n - row_start;

    seq_t bin = binomial(i, j);
    seq_t p7  = ipow_i32((seq_t)7, i - j);
    seq_t p9  = ipow_i32((seq_t)9, j);
    seq_t res = bin * p7 * p9;
    return res;
}
