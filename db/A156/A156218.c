/*
 * A156218  Denominator of Euler(n, 1/9).
 * https://oeis.org/A156218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 3^(2n) * 2^{v2(n+1)}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  v2(k) = largest e such that 2^e divides k */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* exponent for 3: 2*n */
    seq_t exp3 = (seq_t)n * (seq_t)2;
    /* compute 3^(2n) */
    seq_t val = ipow((seq_t)3, exp3);

    /* compute v2(n+1) */
    uint64_t t = n + (uint64_t)1;
    uint64_t exp2 = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        exp2++;
    }

    /* multiply by 2^exp2 */
    seq_t pow2 = ipow_idx((seq_t)2, (idx_t)exp2);
    seq_t result = seq_mul(val, pow2);
    return result;
}
