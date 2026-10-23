/*
 * A130888  Triangle read by rows, A051731(n,k) dot (1, 3, 7, 15, ...) with
 *          like numbers of terms.
 * https://oeis.org/A130888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(r,k) = 2^k-1 if k divides r, else 0 (triangle read by rows)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)                       /* off-by-one safety; a(1) is first term */
        return (seq_t)0;

    /* Find row r such that the triangular number T(r)=r(r+1)/2 >= n. */
    idx_t r = 1;
    for (;; ++r)
    {
        __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
        if ((idx_t)tri >= n)
            break;
    }

    /* Index of the first element of row r */
    idx_t prev_end = (idx_t)(((__uint128_t)(r - 1) * r) / 2);
    idx_t k = n - prev_end;                    /* 1‑based column index */

    if (r % k == 0)                            /* k divides r? */
    {
        seq_t val = pow_seq((seq_t)2, (int)k);  /* 2^k (big or 64-bit)   */
        return val - (seq_t)1;                 /* 2^k - 1 */
    }
    else
        return (seq_t)0;
}
