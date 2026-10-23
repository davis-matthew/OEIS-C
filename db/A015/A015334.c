/*
 * A015334  Gaussian binomial coefficient [ n,6 ] for q = -11.
 * https://oeis.org/A015334
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,6] for q = -11
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the value for any n ≥ 6 by evaluating the
   product form
        a(n) = ∏_{i=0}^{5}(1-(-11)^{n-i}) / ∏_{i=0}^{5}(1-(-11)^{6-i})
   using only unsigned arithmetic.  The cancelation is performed with
   GCDs so that intermediate results stay small enough for the 64‑bit
   build, while the arbitrary‑precision build produces the exact value. */

#include "../utils/big_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Return 0 for n < 6 (undefined behaviour in the sequence). */
    if (n < 6) return 0;

    /* 1.  Compute 11^(n-5).  This is the smallest power needed. */
    seq_t pow_base = big_pow((seq_t)11, (seq_t)(n - 5));

    /* 2.  Build powers 11^(n-5), 11^(n-4), … , 11^n. */
    seq_t pow_abs[6];
    pow_abs[5] = pow_base;               /* 11^(n-5) */
    for (idx_t i = 4; i < 6; --i) {
        pow_abs[i] = pow_abs[i + 1] * (seq_t)11;   /* multiply by 11 */
    }

    /* 3.  Compute the absolute numerators:
          abs_num[i] = |1 - (-11)^{n-i}| = 11^{n-i} ± 1
          (minus if exponent is even, plus if odd). */
    seq_t abs_num[6];
    for (idx_t i = 0; i < 6; ++i) {
        if (((n - i) & 1) == 0)          /* even exponent */
            abs_num[i] = pow_abs[i] - (seq_t)1;
        else                             /* odd exponent  */
            abs_num[i] = pow_abs[i] + (seq_t)1;
    }

    /* 4.  Absolute denominator constants |1-(-11)^{6-i}|. */
    seq_t d[6] = { (seq_t)1771560,
                   (seq_t)161052,
                   (seq_t)14640,
                   (seq_t)1332,
                   (seq_t)120,
                   (seq_t)12 };

    /* 5.  Cancel common factors between numerators and denominators. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 6; ++i) {
        seq_t x = abs_num[i];
        for (idx_t j = 0; j < 6; ++j) {
            seq_t g = seq_gcd(x, d[j]);
            if (g > (seq_t)1) {
                x /= g;
                d[j] /= g;
            }
        }
        result *= x;          /* all d[j] should become 1 after the loop */
    }

    return result;
}
