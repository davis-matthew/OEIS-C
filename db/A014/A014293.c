/*
 * A014293  a(n) = n^(n+1) - n + 1.
 * https://oeis.org/A014293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^(n+1) - n + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 100 (2026-11-01)
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
    seq_t exp  = nseq + (seq_t)1;
    seq_t power = ipow(nseq, exp);
    seq_t res   = power - nseq + (seq_t)1;
    return res;
}
