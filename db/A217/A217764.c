/*
 * A217764  Array defined by a(n,k) = floor((k+2)/2)*3^n - floor((k+1)/2)*2^n,
 *          read by antidiagonals.
 * https://oeis.org/A217764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{d=0}^{∞} [if n < (d+1)(d+2)/2] { a(i,k) } where
 *            a(i,k) = floor((k+2)/2) * 3^i - floor((k+1)/2) * 2^i,
 *            antidiagonal enumeration: (i,j) = (d - offset, offset) for the
 *            nth term
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Return the nth term of OEIS A217764 */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that n < (d+1)(d+2)/2.
       cum holds the total number of elements before diagonal d. */
    idx_t d = 0, cum = 0;
    while (n >= cum + d + 1) {
        cum += d + 1;
        d++;
    }

    /* Position of n within diagonal d. */
    idx_t offset = n - cum;
    idx_t i = d - offset;   /* row index */
    idx_t k = offset;       /* column index */

    /* Compute the formula components. */
    seq_t pow3_val = pow3(i);
    seq_t pow2_val = pow2(i);

    /* floor((k+2)/2) and floor((k+1)/2) as integer divisions. */
    seq_t mul3 = ((seq_t)(k + 2) / 2) * pow3_val;
    seq_t mul2 = ((seq_t)(k + 1) / 2) * pow2_val;

    return mul3 - mul2;
}
