/*
 * A030649  Dimensions of multiples of minimal representation of complex Lie
 *          algebra E7.
 * https://oeis.org/A030649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (1/10950439500)*(n+9)*binomial(n+17,4)*binomial(n+4,4)*binomial(n+13,9)^2
 * Range:     u8 none, u16 none, u32 none, u64 n <= 30, u128 n <= 196,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

static void mul_reduce(seq_t *num, seq_t *den, seq_t factor) {
    seq_t g = gcd(factor, *den);
    if (g != 1) {
        factor /= g;
        *den   /= g;
    }
    *num *= factor;
    g = gcd(*num, *den);
    if (g != 1) {
        *num /= g;
        *den /= g;
    }
}

seq_t a(idx_t n) {
    seq_t num = 1;
    seq_t den = (seq_t)10950439500ULL;

    /* factor (n+9) */
    seq_t f = (seq_t)n + 9;
    mul_reduce(&num, &den, f);

    /* factor binomial(n+17,4) */
    f = binomial((idx_t)n + 17, 4);
    mul_reduce(&num, &den, f);

    /* factor binomial(n+4,4) */
    f = binomial((idx_t)n + 4, 4);
    mul_reduce(&num, &den, f);

    /* factor binomial(n+13,9) */
    f = binomial((idx_t)n + 13, 9);
    mul_reduce(&num, &den, f);

    /* factor binomial(n+13,9) again (square) */
    mul_reduce(&num, &den, f);

    /* After reductions, den should be 1; return the computed value */
    return num;
}
