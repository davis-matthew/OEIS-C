/*
 * A120800  a(n) = 3^(n^2) + 2^(n^2).
 * https://oeis.org/A120800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n^2) + 2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 25 (2026-11-01)
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
    seq_t exp = nseq * nseq;
    seq_t p3 = ipow((seq_t)3, exp);
    seq_t p2 = ipow((seq_t)2, exp);
    return p3 + p2;
}
