/*
 * A226238  a(n) = (n^n - n)/(n - 1).
 * https://oeis.org/A226238
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^n - n)/(n - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 384 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* For n < 2 the definition is not in the offset range; return 0. */
    if (n < 2)
        return 0;

    seq_t pow_val = ipow_u64_idx(n, n);          /* n^n  */
    seq_t n_val  = (seq_t)n;                 /* cast to seq_t   */
    return (pow_val - n_val) / (n_val - 1);  /* (n^n - n)/(n-1) */
}
