/*
 * A074532  a(n) = 2^n + 4^n + 5^n.
 * https://oeis.org/A074532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 4^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 171 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

/* OEIS sequence A074532 */
seq_t a(idx_t n)
{
    seq_t t2 = ipow_idx((seq_t)2, n);
    seq_t t4 = ipow_idx((seq_t)4, n);
    seq_t t5 = ipow_idx((seq_t)5, n);
    return t2 + t4 + t5;
}
