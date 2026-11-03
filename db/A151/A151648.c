/*
 * A151648  Number of permutations of 5 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56^n - (5*n + 1)*21^n + binomial(5*n+1, 2)*6^n -
 *            binomial(5*n+1, 3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 190 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for offset 1 but safe */

    seq_t nn = (seq_t)n;

    /* powers */
    seq_t pow56 = ipow((seq_t)56, nn);
    seq_t pow21 = ipow((seq_t)21, nn);
    seq_t pow6  = ipow((seq_t)6, nn);

    /* (5*n+1) fits easily in an unsigned int */
    unsigned int coeff = (unsigned int)(5 * n + 1);

    /* terms of the formula */
    seq_t term2 = mul_small(pow21, coeff);
    seq_t term3 = seq_mul(binomial(5 * n + 1, 2), pow6);
    seq_t term4 = binomial(5 * n + 1, 3);

    /* compute result */
    seq_t res = pow56 - term2 + term3 - term4;
    return res;
}
