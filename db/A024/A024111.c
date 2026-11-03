/*
 * A024111  a(n) = 9^n - n^10.
 * https://oeis.org/A024111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 293 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t pow9  = ipow((seq_t)9, nseq);
    seq_t pow10 = ipow(nseq, (seq_t)10);
    return pow9 - pow10;
}
