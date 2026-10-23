/*
 * A038297  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*7^j.
 * https://oeis.org/A038297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*9^(i-j)*7^j, where i*(i+1)/2 <= n <
 *            (i+1)*(i+2)/2 and j = n – i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row index i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t lo = 0, hi = 1;
    /* Expand hi until it's large enough. */
    while ((__int128)hi * (hi + 1) / 2 <= n) {
        hi <<= 1;
        if (hi == 0) {               /* overflow guard */
            hi = ~((idx_t)0);
            break;
        }
    }

    idx_t i = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            i = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    /* Column index j within row i. */
    idx_t tri_i = i * (i + 1) / 2;
    idx_t j = n - tri_i;

    /* Compute the needed powers. */
    seq_t pow9 = pow_uint(9, i - j);
    seq_t pow7 = pow_uint(7, j);

    /* Binomial coefficient. */
    seq_t bin = binomial(i, j);

    /* Final result. */
    return bin * pow9 * pow7;
}
