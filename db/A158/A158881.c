/*
 * A158881  a(n) = (n*2^n + 1)^(n-1).
 * https://oeis.org/A158881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*2^n + 1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 54 (2026-11-01)
 * Helpers:   ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 by definition */

    seq_t pow_n  = pow2(n);                /* 2^n */
    seq_t base   = ((seq_t)n) * pow_n + (seq_t)1; /* n*2^n + 1 */
    seq_t exp    = (seq_t)(n - 1);         /* n-1 */

    return ipow(base, exp);             /* (n*2^n + 1)^(n-1) */
}
