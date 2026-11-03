/*
 * A223906  Poly-Cauchy numbers of the second kind -hat c_3^(-n).
 * https://oeis.org/A223906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 3^(n+1) + 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 975 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Poly-Cauchy numbers of the second kind –hat c_3^(-n) for n >= 1 */
seq_t a(idx_t n)
{
    /* As per the conjectured formula:
       a(n) = 2^(n+1) + 3^(n+1) + 4^n
    */
    seq_t t1 = ipow_idx((seq_t)2, n + 1);
    seq_t t2 = ipow_idx((seq_t)3, n + 1);
    seq_t t3 = ipow_idx((seq_t)4, n);
    return t1 + t2 + t3;
}
