/*
 * A157499  Denominator of Euler(n, 1/31).
 * https://oeis.org/A157499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 31^n * 2^{v2(n+1)} (where v2(k) is the exponent of 2
 *            dividing k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* compute 31^n */
    seq_t pow31 = ipow((seq_t)31, (seq_t)n);

    /* count trailing zeros of n+1  →  v2(n+1) */
    idx_t t = 0;
    idx_t m = n + 1;
    while ((m & ((idx_t)1)) == 0) {
        ++t;
        m >>= 1;
    }

    /* compute 2^t */
    seq_t pow2 = ipow((seq_t)2, (seq_t)t);

    /* result = 31^n * 2^t */
    return seq_mul(pow31, pow2);
}
