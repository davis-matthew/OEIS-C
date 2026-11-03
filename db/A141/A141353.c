/*
 * A141353  a(n) = Catalan(n) + 2^n - 0^n.
 * https://oeis.org/A141353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = catalan(n) + 2^n - 0^n
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t cat   = catalan(n);
    seq_t p2    = pow2(n);
    seq_t result = cat + p2;
    if (n == 0)                                      /* 0^0 = 1 */
        result -= 1;
    return result;
}
