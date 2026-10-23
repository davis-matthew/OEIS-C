/*
 * A118185  Triangle T(n,k) = 4^(k*(n-k)) for n>=k>=0, read by rows.
 * https://oeis.org/A118185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4^(k*(r-k)), where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 39, u64 n <= 81, u128 n <= 143,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine the row number r that contains the index n.
       Triangular numbers t_r = r*(r+1)/2 serve as boundaries. */
    idx_t r = 0;
    idx_t t = 0;                     /* t is the first index of the current row */
    while (((unsigned __int128)t + (r + 1)) <= n) {
        t += r + 1;                  /* t becomes the start index of the next row */
        r++;
    }

    /* Position k within the found row (0‑based). */
    idx_t k = n - t;

    /* Exponent e = k*(r-k).  The product fits in 64‑bit, but we use 128‑bit
       for safety against overflow. */
    __int128 prod = (__int128)k * (r - k);
    idx_t e = (idx_t)prod;

    /* Return 4^e = 2^(2*e).  pow2 returns 2^exp as a seq_t. */
    return pow2(2 * e);
}
