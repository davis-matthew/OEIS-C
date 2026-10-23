/*
 * A035840  Coordination sequence for A_14 lattice.
 * https://oeis.org/A035840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 44, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A035840: Coordination sequence for A_14 lattice
 * Generated from the rational generating function:
 *   (x^14 + 196*x^13 + 8281*x^12 + 132496*x^11 + 1002001*x^10 + 4008004*x^9
 *    + 9018009*x^8 + 11778624*x^7 + 9018009*x^6 + 4008004*x^5 + 1002001*x^4
 *    + 132496*x^3 + 8281*x^2 + 196*x + 1) / (1 - x)^14
 * The coefficient of x^n in this series is:
 *   a(n) = Σ_{i=0}^{14} c_i * binom(n - i + 13, 13)
 *   where c_0..c_14 are the numerator coefficients above.
 *
 * The implementation uses the provided binom() helper to compute
 * small binomial coefficients exactly.
 */
#include "../utils/binom.h"

/* Sequence terms a(n) for n >= 0 */
seq_t a(idx_t n)
{
    static const seq_t coeffs[15] = {
        1,          /* x^0 */
        196,        /* x^1 */
        8281,       /* x^2 */
        132496,     /* x^3 */
        1002001,    /* x^4 */
        4008004,    /* x^5 */
        9018009,    /* x^6 */
        11778624,   /* x^7 */
        9018009,    /* x^8 */
        4008004,    /* x^9 */
        1002001,    /* x^10 */
        132496,     /* x^11 */
        8281,       /* x^12 */
        196,        /* x^13 */
        1           /* x^14 */
    };

    seq_t sum = 0;
    for (idx_t i = 0; i < 15; ++i) {
        if (n >= i) {
            idx_t top   = n - i + 13;   /* n-i+13 >= 13 because n>=i */
            seq_t bin   = binom(top, 13);
            sum += coeffs[i] * bin;
        }
    }
    return sum;
}
