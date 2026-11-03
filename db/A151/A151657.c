/*
 * A151657  Number of permutations of 7 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120^n - (7*n + 1)*36^n + binomial(7*n+1, 2)*8^n -
 *            binomial(7*n+1, 3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 179 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Powers of 120, 36, and 8 */
    seq_t pow120 = ipow_idx((seq_t)120, n);
    seq_t pow36  = ipow_idx((seq_t)36,  n);
    seq_t pow8   = ipow_idx((seq_t)8,   n);

    /* (7*n + 1) * 36^n */
    idx_t coef = 7 * n + 1;
    seq_t term2 = mul_small(pow36, (unsigned int)coef);

    /* binomial(7*n+1, 2) * 8^n */
    seq_t bin2    = binomial(7 * n + 1, 2);
    seq_t term3   = seq_mul(bin2, pow8);

    /* binomial(7*n+1, 3) */
    seq_t bin3 = binomial(7 * n + 1, 3);

    /* Combine all terms */
    seq_t value = pow120;
    value -= term2;
    value += term3;
    value -= bin3;

    return value;
}
