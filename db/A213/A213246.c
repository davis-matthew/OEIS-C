/*
 * A213246  Number of nonzero elements in GF(2^n) that are 9th powers.
 * https://oeis.org/A213246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1) / gcd(2^n - 1, 9) (n ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 985 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* for any valid n ≥ 1 */
    seq_t power = ipow_idx((seq_t)2, n);   /* 2^n */
    seq_t M = power - (seq_t)1;           /* M = 2^n - 1 */
    seq_t g = gcd(M, (seq_t)9);           /* gcd(M, 9) */
    return M / g;
}
