/*
 * A024107  a(n) = 9^n - n^6.
 * https://oeis.org/A024107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - n^6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 397 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 9^n */
    seq_t pow9 = ipow((seq_t)9, (seq_t)n);

    /* compute n^6 as (n^3)^2 */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;
    seq_t n3   = n2 * nseq;
    seq_t n6   = n3 * n3;

    return pow9 - n6;
}
