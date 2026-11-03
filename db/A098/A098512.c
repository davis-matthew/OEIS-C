/*
 * A098512  Second column and subdiagonal of number triangle A098509.
 * https://oeis.org/A098512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n) / gcd(n, C(n)) where C(n) is the Catalan number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Catalan number C(n) = binomial(2n, n) / (n + 1) */
    seq_t binom    = binomial(2 * n, n);
    seq_t catalan  = binom / (((seq_t)n) + (seq_t)1);

    /* gcd(n, C(n)) */
    seq_t g = gcd(catalan, (seq_t)n);

    return catalan / g;
}
