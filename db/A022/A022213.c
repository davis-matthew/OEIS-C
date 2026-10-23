/*
 * A022213  Gaussian binomial coefficients [ n,6 ] for q = 5.
 * https://oeis.org/A022213
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..6} (5^(n-i+1)-1)/(5^i-1)
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the Gaussian binomial coefficient [n,6] with q = 5.
    It works for any seq_t type (arbitrary‑precision or native integer)
    and is safe for all n ≥ 6 that fit in seq_t.  For native types the
    routine relies on exact cancellations to keep intermediate values
    within range. */
#include "../utils/pow_seq.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; returning 0 is harmless. */
    if (n < 6) return 0;

    seq_t result = 1;

    /* Multiply the six factors while cancelling common divisors
       to avoid overflow. */
    for (idx_t i = 1; i <= 6; ++i) {
        /* Numerator 5^(n-i+1) – 1 */
        seq_t num = pow_seq((seq_t)5, (int)(n - i + 1));
        num -= 1;
        /* Denominator 5^i – 1 */
        seq_t den = pow_seq((seq_t)5, (int)i);
        den -= 1;

        /* Cancel common factors between current result and denominator */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* Cancel remaining common factors between numerator and denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply and divide by the reduced parts */
        result *= num;
        if (den != 1)
            result /= den;   /* should never happen, but keeps the code robust */
    }

    return result;
}
