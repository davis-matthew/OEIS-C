/*
 * A022243  Gaussian binomial coefficients [ n,3 ] for q = 8.
 * https://oeis.org/A022243
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Prod_{i=1..3} (8^(n-i+1)-1)/(8^i-1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Gaussian binomial coefficient [n,3] for q = 8. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

/* External linkage function as requested */
seq_t a(idx_t n) {
    /* The OEIS offset is 3, but we simply compute for all n >= 3.
       For n < 3 the result is not defined; return 0. */
    if (n < 3)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    const seq_t base = (seq_t)8;

    for (idx_t i = 1; i <= 3; ++i) {
        /* numerator: 8^(n-i+1) - 1 */
        seq_t num = big_pow(base, (seq_t)(n - i + 1)) - (seq_t)1;
        /* denominator: 8^i - 1 */
        seq_t den = big_pow(base, (seq_t)i) - (seq_t)1;

        /* Cancel common factors between numerator and denominator */
        seq_t g = gcd(num, den);
        if (g != (seq_t)1) {
            num /= g;
            den /= g;
        }

        /* Further cancel with the current result */
        g = gcd(result, den);
        if (g != (seq_t)1) {
            result /= g;
            den /= g;
        }

        /* Now multiply and divide, result division is exact */
        result = (result * num) / den;
    }

    return result;
}
