/*
 * A081684  a(n) = 5^n - 4^n - 3^n - 2^n + 3.
 * https://oeis.org/A081684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 5^n - 4^n - 3^n - 2^n + 3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t p5 = ipow((seq_t)5, nn);
    seq_t p4 = ipow((seq_t)4, nn);
    seq_t p3 = ipow((seq_t)3, nn);
    seq_t p2 = ipow((seq_t)2, nn);
    seq_t result = p5 - p4 - p3 - p2 + (seq_t)3;
    return result;
}
