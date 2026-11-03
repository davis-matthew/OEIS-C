/*
 * A171801  O.g.f.: Sum_{n>=0} (n+1)*2^(n^2)*x^n/(1 - 2^n*x)^n.
 * https://oeis.org/A171801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * ((n+1)*2^n + 2) * (2^n + 1)^(n-2) for n>0, with
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* 2^n */
    seq_t pow2 = ipow_u64_idx(2, n);
    /* (2^n + 1)^(n-2) */
    seq_t term2 = ipow(pow2 + (seq_t)1, (seq_t)(n - 2));
    /* ((n+1)*2^n + 2) */
    seq_t inner = ((seq_t)(n + 1)) * pow2 + (seq_t)2;
    /* Final product */
    seq_t result = pow2 * inner;
    result = result * term2;
    return result;
}
