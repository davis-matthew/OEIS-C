/*
 * A177068  a(n) = 10^n + n^10.
 * https://oeis.org/A177068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + n^10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t pow10n = pow10(n);          /* 10^n  */
    seq_t nseq   = (seq_t)n;          /* convert index to seq_t */
    seq_t n10    = ipow_idx(nseq, 10); /* n^10   */
    return pow10n + n10;              /* sum gives a(n) */
}
