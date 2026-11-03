/*
 * A008353  a(n) = 2^n*(2^(n+1) - n - 1).
 * https://oeis.org/A008353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * ( 2^(n+1) - n - 1 )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1334 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 2^(n+1) */
    seq_t pow_n      = ipow((seq_t)2, n);
    seq_t pow_n_plus = ipow((seq_t)2, n + 1);

    /* subtract n + 1 from 2^(n+1) */
    seq_t sub = pow_n_plus - ((seq_t)n + 1);

    /* multiply */
    return pow_n * sub;
}
