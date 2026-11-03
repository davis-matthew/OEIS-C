/*
 * A089156  a(n) = A069722(n+1)^2.
 * https://oeis.org/A089156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = 2^(2*n) * C(2*n,n)^2 (C = binomial)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0U)   /* offset 0 */
        return 0;

    idx_t twon = 2 * n;           /* compute 2*n */
    seq_t c    = binomial(twon, n);  /* C(2n, n) */
    seq_t res  = c * c;           /* square the binomial */
    res       = res * pow2(twon); /* multiply by 2^(2n) */
    return res;
}
