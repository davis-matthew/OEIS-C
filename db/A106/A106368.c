/*
 * A106368  Necklaces with n beads of 6 colors, no 2 adjacent beads the same
 *          color.
 * https://oeis.org/A106368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6 * a(n) = (1/n) * Sum_{d | n} φ(n/d) * (5*(-1)^d + 5^d),
 *            for n > 1 * * The implementation precomputes 5^d by fast
 *            exponentiation and φ(k) * by trial division. No external
 *            libraries are required; all * calculations use the user‑supplied
 *            types seq_t and idx_t
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   phi_calc, pow5, term_for_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_calc.h"
#include "../utils/pow5.h"
#include "../utils/term_for_divisor.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;      /* base case */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            sum += term_for_divisor(d, n);
            idx_t d2 = n / d;
            if (d2 != d)
                sum += term_for_divisor(d2, n);
        }
    }
    return sum / (seq_t)n;
}
