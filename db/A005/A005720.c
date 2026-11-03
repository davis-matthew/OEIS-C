/*
 * A005720  Quadrinomial coefficients.
 * https://oeis.org/A005720
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(n,2) + 7*binomial(n,3) + 10*binomial(n,4) +
 *            5*binomial(n,5) + binomial(n,6)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 37 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t r = binomial(n, 2);
    r += (seq_t)7 * binomial(n, 3);
    r += (seq_t)10 * binomial(n, 4);
    r += (seq_t)5 * binomial(n, 5);
    r += binomial(n, 6);
    return r;
}
