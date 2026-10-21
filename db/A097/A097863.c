/*
 * A097863  Sum of 5-infinitary divisors of n.
 * https://oeis.org/A097863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: The sequence is the sum of the 5‑infinitary divisors of n.
 * For n = ∏ p_i^{e_i} let e_i be written in base 5:
 *       e_i = d_0 + d_1·5 + d_2·5² + …   (0 ≤ d_j ≤ 4)
 * For each digit d_j the contribution to the sum is
 *       S_j = 1 + p_i^{d_j·5^j} + p_i^{2d_j·5^j} + … + p_i^{d_j·5^j}
 *   (if d_j = 0 then S_j = 1).
 * The value a(n) equals the product of all S_j for all primes p_i.
 * The function below implements this multiplicative formula.
 */

#include <stdint.h>

static inline seq_t pow5_seq(seq_t x) {
    /* returns x^5  (x*x*x*x*x) */
    seq_t r = x * x;   /* x² */
    r *= r;           /* x⁴ */
    r *= x;           /* x⁵ */
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not defined, but safe */

    seq_t result = 1;
    idx_t m = n;

    /* trial division factorisation – sufficient for n up to ~10⁵ */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* compute contribution of p^e */
            seq_t pow_val = p;          /* p^(5^0) */
            seq_t total = 1;            /* product over digits */

            idx_t rem = e;
            while (rem > 0) {
                uint8_t digit = rem % 5;   /* current base‑5 digit */
                if (digit > 0) {
                    seq_t sum = 1;
                    seq_t powk = 1;
                    for (uint8_t k = 1; k <= digit; ++k) {
                        powk *= pow_val;
                        sum += powk;
                    }
                    total *= sum;
                }
                rem /= 5;            /* move to next digit */

                /* advance pow_val to p^(5^(next)) = (p^(5^current))^5 */
                pow_val = pow5_seq(pow_val);
            }
            result *= total;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1) {
        seq_t pow_val = m;          /* m^(5^0) = m */
        result *= (1 + pow_val);    /* exponent e = 1 → digit 1 */
    }

    return result;
}
