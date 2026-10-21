/*
 * A066968  Denominators of b(n) = (1/16)^n*(4/(8*n+1) - 2/(8*n+4) - 1/(8*n+5)
 *          - 1/(8*n+6)).
 * https://oeis.org/A066968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 24,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of the Machin‐style term
   b(n) = (1/16)^n * (4/(8*n+1) - 2/(8*n+4) - 1/(8*n+5) - 1/(8*n+6)) */
#include <stdint.h>
#include "../utils/seq_pow.h"

/* Greatest common divisor for seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Least common multiple for seq_t */
static inline seq_t lcm_seq(seq_t a, seq_t b) {
    if (a == 0 || b == 0) return 0;
    return (a / seq_gcd(a, b)) * b;
}

seq_t a(idx_t n) {
    seq_t D1 = (seq_t)8 * n + 1;
    seq_t D2 = (seq_t)8 * n + 4;
    seq_t D3 = (seq_t)8 * n + 5;
    seq_t D4 = (seq_t)8 * n + 6;

    /* LCM of the four denominators */
    seq_t L = lcm_seq(D1, D2);
    L = lcm_seq(L, D3);
    L = lcm_seq(L, D4);

    /* Numerator of the bracketed expression */
    seq_t N = (seq_t)4 * L / D1
            - (seq_t)2 * L / D2
            - L / D3
            - L / D4;

    /* 16^n = 2^(4n) */
    seq_t pow2 = seq_pow((seq_t)2, 4 * n);
    seq_t denom_total = L * pow2;

    /* Reduce the fraction */
    seq_t g = seq_gcd(N, denom_total);
    return denom_total / g;
}
