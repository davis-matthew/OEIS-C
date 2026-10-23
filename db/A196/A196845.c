/*
 * A196845  Table of elementary symmetric function a_k(3,4,...,n+2) (no 1 and
 *          2).
 * https://oeis.org/A196845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the coefficient a_k of x^k in * ∏_{i=3}^{r+2} (1 + i x)
 *            , * where r and k are obtained from the linear index n: * r =
 *            floor( (sqrt(8 n+1) – 1) / 2 ), * k = n – r(r+1)/2 . * The
 *            sequence starts 1,1,3,1,7,12,1,
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   elementary_symmetric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/elementary_symmetric.h"

seq_t a(idx_t n)
{
    /* Determine r so that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    idx_t k = n - r * (r + 1) / 2;
    if (k == 0) return (seq_t)1;          /* a_0 = 1 */

    return elementary_symmetric(r, k);
}
