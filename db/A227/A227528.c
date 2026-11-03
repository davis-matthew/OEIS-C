/*
 * A227528  a(n) = numerator of r(n) = 2*(3*n+2)!/((2*n)!*2^n), n>=0.
 * https://oeis.org/A227528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of r(n) = 2*(3*n+2)!/((2*n)!*2^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 308 (2026-11-01)
 * Helpers:   ctz, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ctz.h"
#include "../utils/pow2.h"

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
    seq_t odd_part = 1;          /* product of the odd parts of the factors */
    idx_t exp_two  = 0;          /* total exponent of 2 in the product */

    idx_t start = 2 * n + 1;
    idx_t end   = 3 * n + 2;

    for (idx_t k = start; k <= end; ++k) {
        idx_t t = ctz(k);           /* number of factors 2 in k */
        exp_two += t;
        idx_t odd_k = k >> t;           /* k with all 2's removed */
        odd_part = mul_small(odd_part, (unsigned int)odd_k);
    }

    /* leading factor 2 from the definition contributes one more 2 */
    exp_two += 1;

    /* net exponent of 2 after division by 2^n */
    idx_t net_twos = 0;
    if (exp_two > n)
        net_twos = exp_two - n;

    if (net_twos > 0) {
        seq_t pwr = pow2(net_twos);
        odd_part = seq_mul(odd_part, pwr);
    }

    return odd_part;
}
