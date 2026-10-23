/*
 * A140930  Number of 7 X 10 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 7,10,n can be permuted, see
 *          formula.
 * https://oeis.org/A140930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ∏_{i=0}^{6} ( binomial(n+16, n+i) * i! / (n+16-i)^(6-i) )
 * Range:     u8 n <= 0, u16 n <= 0, u32 none, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    const idx_t p = n;
    const idx_t q = 10;   /* unused directly but part of the definition */
    const idx_t r = 7;
    idx_t s = p + q + r - 1;   /* s = n + 16 */

    /* factorials 0! … 6! */
    static const seq_t fact[7] = {1, 1, 2, 6, 24, 120, 720};

    seq_t num = 1;
    seq_t den = 1;

    for (idx_t i = 0; i < r; ++i) {
        /* choose: binomial(s, p + i) */
        seq_t bin = binomial(s, p + i);
        seq_t t_num = bin * fact[i];

        /* denominator: (s - i)^(r - i - 1) */
        idx_t exp = r - i - 1;
        seq_t t_den = 1;
        if (exp > 0) {
            seq_t base = (seq_t)(s - i);
            for (idx_t e = 0; e < exp; ++e)
                t_den *= base;
        }

        /* cancel common factors to avoid overflow */
        seq_t g = gcd(t_num, den);
        t_num /= g;
        den /= g;

        g = gcd(num, t_den);
        num /= g;
        t_den /= g;

        /* multiply the reduced fractions */
        num *= t_num;
        den *= t_den;

        /* final reduction */
        g = gcd(num, den);
        num /= g;
        den /= g;
    }

    /* after the loop den should be 1 */
    return num;
}
