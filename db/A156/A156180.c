/*
 * A156180  Denominator of Euler(n,1/3).
 * https://oeis.org/A156180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * 2^{v2(n+1)} where v2(k) = number of trailing zeros
 *            in the binary representation of k. This yields the denominators
 *            of Euler(n,1/3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* compute v2(n+1) */
    idx_t m = n + 1;
    idx_t shift = 0;
    while ((m & 1) == 0) {
        shift++;
        m >>= 1;
    }

    /* powers of 3 and 2 as seq_t */
    seq_t pow3 = ipow_idx((seq_t)3, n);
    seq_t pow2 = ipow_idx((seq_t)2, shift);

    /* a(n) = 3^n * 2^{v2(n+1)} */
    return seq_mul(pow3, pow2);
}
