/*
 * A166232  a(n) = A166231(n)/4^n.
 * https://oeis.org/A166232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(C(n,2)) * ∑_{k=0..⌊n/2⌋} C(n,2k) * 2^(-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n / 2;

    /* exponent e = C(n,2) - m = n*(n-1)/2 - m */
    idx_t e = (n * (n - 1)) / 2 - m;

    /* sum = Σ_{even j} C(n,j) * 2^(m - j/2)  */
    seq_t sum = 0;

    seq_t bin = 1;                /* C(n,0) */
    for (idx_t j = 0; j <= n; ++j) {
        if ((j & 1) == 0) {          /* even j */
            idx_t k = j >> 1;        /* k = j/2 */
            idx_t shift = m - k;     /* shift = m - k */
            seq_t term = bin * pow2(shift);
            sum += term;
        }
        if (j == n) break;           /* avoid overflow in update */
        /* update bin = C(n, j+1) from C(n,j) */
        bin = (bin * (seq_t)(n - j)) / (seq_t)(j + 1);
    }

    /* a(n) = sum * 2^e */
    return sum * pow2(e);
}
