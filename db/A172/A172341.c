/*
 * A172341  a(n) = ((2^n+1)^n + (2^n-1)^n)/2.
 * https://oeis.org/A172341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^n+1)^n + (2^n-1)^n)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1, so a(1) is the first term) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle n < 1 gracefully (undefined behaviour for offset).  */
    if (n == 0)
        return 0;

    /* Compute 2^n */
    seq_t two_n = pow2(n);

    /* Construct (2^n + 1) and (2^n - 1) */
    seq_t plus  = two_n + (seq_t)1;
    seq_t minus = two_n - (seq_t)1;

    /* Raise to the n-th power */
    seq_t p1 = ipow_idx(plus, n);
    seq_t p2 = ipow_idx(minus, n);

    /* Add and divide by 2 */
    return (p1 + p2) / (seq_t)2;
}
