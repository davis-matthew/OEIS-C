/*
 * A200791  Number of 0..n arrays x(0..8) of 9 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{10} c_k * binom(n - k + 9, 9), where c = [512,
 *            9729, 37570, 39065, 11862, 551, 124, -45, 10, -1]. This follows
 *            from the generating function x*(512 + 9729*x + 37570*x^2 +
 *            39065*x^3 + 11862*x^4 + 551*x^5 + 124*x^6 - 45*x^7 + 10*x^8 -
 *            x^9) / (1 - x)^10. The binomial coefficients bino...
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 135, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute binomial coefficient C(m,9) for any m>=0 */
static inline seq_t binom9(idx_t m)
{
    if (m < 9)
        return 0;
    seq_t res = 1;
    /* Multiply (m-8) through m and divide by 9! iteratively */
    for (idx_t i = 1; i <= 9; ++i)
    {
        res = res * (m - 9 + i) / i;
    }
    return res;
}

/* Coefficients c_k for k = 1..10 */
static const int coeffs[10] = {
    512, 9729, 37570, 39065, 11862,
    551, 124, -45, 10, -1
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Index below offset, unspecified behaviour */

    seq_t pos = 0;
    seq_t neg = 0;

    for (int idx = 0; idx < 10; ++idx)
    {
        idx_t k = (idx_t)(idx + 1);                     /* k = 1..10 */
        idx_t m = n - k + 9;                            /* argument for binomial */
        seq_t bin = binom9(m);                          /* C(m,9), 0 if m<9 */

        int c = coeffs[idx];
        seq_t term = bin * (c >= 0 ? (seq_t)c : (seq_t)(-c));

        if (c >= 0)
            pos += term;
        else
            neg += term;
    }

    /* pos >= neg because the overall coefficient sum (99377) is positive */
    return pos - neg;
}
