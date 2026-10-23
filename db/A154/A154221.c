/*
 * A154221  A simple Pascal-like triangle.
 * https://oeis.org/A154221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 + 2^(k-1)*2^(r-k-1) where r = ⌊(√(8n+1)-1)/2⌋, k = n -
 *            r(r+1)/2. Edge entries (k=0 or k=r) are 1
 * Range:     u8 n <= 55, u16 n <= 171, u32 n <= 595, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute integer floor sqrt of 8*n + 1 */
    idx_t t = 8ull * n + 1ull;
    idx_t sq = 0;
    while ((sq + 1) <= t / (sq + 1))
        ++sq;

    idx_t r = (sq - 1) / 2;          /* row index */
    idx_t prev = r * (r + 1) / 2;    /* number of terms before row r */
    idx_t k = n - prev;              /* column index within row r */

    if (k == 0 || k == r)
        return (seq_t)1;

    /* compute 2^(k-1) and 2^(r-k-1) using the provided pow_uint helper */
    seq_t a_val = pow_uint(2ull, k - 1);
    seq_t b_val = pow_uint(2ull, r - k - 1);

    return a_val * b_val + (seq_t)1;
}
