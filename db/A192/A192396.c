/*
 * A192396  Square array T(n, k) = floor(((k+1)^n - (1+(-1)^k)/2)/2) read by
 *          antidiagonals.
 * https://oeis.org/A192396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = floor((( (j+1)^i ) - (1+(-1)^j)/2)/2) where (i,j) are the
 *            antidiagonal coordinates of n: i+j = d, d*(d+1)/2 <= n <
 *            (d+1)(d+2)/2, offset t = n - d*(d+1)/2, i = d - t, j = t
 * Range:     u8 n <= 37, u16 n <= 80, u32 n <= 175, u64 n <= 441,
 *            u128 n <= 1138, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = 0;
    for (;; ++d) {
        unsigned __int128 tri = (unsigned __int128)d * (d + 1) / 2;
        if (tri > n) { d--; break; }
    }

    /* Calculate the offset within the diagonal. */
    idx_t offset = n - (unsigned __int128)d * (d + 1) / 2;
    idx_t row = d - offset;   /* i */
    idx_t col = offset;       /* j */

    /* Parity term: subtract 1 iff col is even. */
    seq_t parity = ((col & 1) == 0) ? (seq_t)1 : (seq_t)0;

    seq_t result;
    if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller */
        unsigned __int128 power = 1;   /* (col+1)^row */
        unsigned __int128 base  = (unsigned __int128)(col + 1);

        for (idx_t e = 0; e < row; ++e) {
            /* If multiplication would overflow 128 bits, set power to max. */
            if (power > (((unsigned __int128)~(unsigned __int128)0) / base))
                { power = (unsigned __int128)~(unsigned __int128)0; break; }
            power *= base;
        }

        if (parity) power -= 1;       /* subtract 1 when col is even */
        power /= 2;                   /* floor division by 2 */
        result = (seq_t)power;
    } else {                           /* arbitrary‑precision build */
        seq_t val = big_pow((seq_t)(col + 1), (seq_t)row);
        if (parity) val -= parity;    /* subtract 1 when col is even */
        val /= (seq_t)2;               /* floor division by 2 */
        result = val;
    }

    return result;
}
