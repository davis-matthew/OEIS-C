/*
 * A015363  Gaussian binomial coefficient [ n,8 ] for q=-7.
 * https://oeis.org/A015363
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} ((-7)^(n-i+1)-1)/((-7)^i-1)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    /* Denominator constants: abs((-7)^i - 1) for i = 1..8 */
    static bool init = false;
    static seq_t den_const[9];
    if (!init) {
        seq_t p = big_pow((seq_t)7, (seq_t)1);   /* 7^1 */
        for (idx_t i = 1; i <= 8; ++i) {
            if (i & 1)          /* i odd: (-7)^i = -7^i, so abs = 7^i + 1 */
                den_const[i] = p + 1;
            else                /* i even: (-7)^i = 7^i, so abs = 7^i - 1 */
                den_const[i] = p - 1;
            p *= (seq_t)7;      /* p = 7^(i+1) for next iteration */
        }
        init = true;
    }

    /* Compute numerators: abs((-7)^(n-i+1)-1) for i = 1..8 */
    seq_t pow = big_pow((seq_t)7, (seq_t)(n - 7));  /* 7^(n-7) */
    seq_t num[8];
    for (idx_t j = 8; j >= 1; --j) {
        idx_t exp = n - j + 1;   /* exponent for this factor */
        if (exp & 1)             /* odd exponent: (-7)^exp = -7^exp */
            num[j - 1] = pow + 1;
        else                     /* even exponent: (-7)^exp =  7^exp */
            num[j - 1] = pow - 1;
        pow *= (seq_t)7;         /* move to exponent +1 for next j */
    }

    /* Local copy of denominators to avoid modifying static constants */
    seq_t den[8];
    for (idx_t i = 1; i <= 8; ++i)
        den[i - 1] = den_const[i];

    /* Simplify the product by cancelling gcds between numerators and denominators */
    for (idx_t i = 0; i < 8; ++i) {
        for (idx_t d = 0; d < 8; ++d) {
            seq_t g = gcd(num[i], den[d]);
            while (g > (seq_t)1) {
                num[i] /= g;
                den[d] /= g;
                if (den[d] == (seq_t)1) break;
                g = gcd(num[i], den[d]);
            }
        }
    }

    /* Multiply remaining numerator factors to obtain the result */
    seq_t result = 1;
    for (idx_t i = 0; i < 8; ++i)
        result *= num[i];

    return result;
}
