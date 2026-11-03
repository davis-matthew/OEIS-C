/*
 * A213244  Number of nonzero elements in GF(2^n) that are 5th powers.
 * https://oeis.org/A213244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) / gcd(2^n - 1, 5)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 838 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑zero elements in GF(2^n) that are 5th powers */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* M = 2^n - 1 */
    seq_t M = ipow_idx((seq_t)2, n);
    M -= (seq_t)1;

    /* g = gcd(M, 5) */
    seq_t g = gcd(M, (seq_t)5);

    /* result = M / g */
    return M / g;
}
