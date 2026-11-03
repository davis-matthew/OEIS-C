/*
 * A203477  a(n) = Product_{0 <= i < j <= n-1} (2^i + 2^j).
 * https://oeis.org/A203477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{0 <= i < j <= n-1} (2^i + 2^j)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* exponent of 2: (n-1)*n*(n-2)/6 */
    idx_t exp = ((n - 1) * n * (n - 2)) / 6;
    seq_t result = pow2(exp);

    seq_t one = 1;
    for (idx_t k = 1; k < n; ++k) {
        seq_t powk   = pow2(k);          /* 2^k  */
        seq_t term   = one + powk;       /* 1 + 2^k */
        seq_t termp  = ipow_idx(term, n - k); /* (1+2^k)^(n-k) */
        result = seq_mul(result, termp);
    }
    return result;
}
