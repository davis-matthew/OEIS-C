/*
 * A161858  Number of reduced words of length n in the Weyl group B_12.
 * https://oeis.org/A161858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Σ_{k=0}^{min(n,156)} c[k] * binom(n-k+11, 11) where c[k]
 *            are the coefficients of Π_{m=1}^{12}(1 - x^{2m})
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 36, u64 n <= 144, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>     /* for stdint types */

static idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute C(N, 11) for N ≥ 11 using cancelation to keep intermediate values small. */
static seq_t binom_n11(idx_t N)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 11; ++i)
    {
        idx_t num = N - 11 + i;          /* numerator factor                           */
        idx_t g   = gcd_idx_t(num, i);   /* cancel common divisor                       */
        num /= g;
        idx_t denom = i / g;
        res = res / (seq_t)denom;        /* exact division                               */
        res = res * (seq_t)num;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Static coefficients of Π(1 - x^{2m}), m = 1..12. */
    static signed char coeff[157];
    static int coeff_init = 0;
    if (!coeff_init)
    {
        for (int i = 0; i < 157; ++i) coeff[i] = 0;
        coeff[0] = 1;                      /* start with 1                              */
        for (int m = 1; m <= 12; ++m)
        {
            int shift = 2 * m;
            for (int j = 156; j >= shift; --j)
                coeff[j] = coeff[j] - coeff[j - shift];
        }
        coeff_init = 1;
    }

    seq_t result = 0;
    int max_j = (n <= 156) ? (int)n : 156;
    for (int j = 0; j <= max_j; ++j)
    {
        int8_t c = coeff[j];
        if (c == 0) continue;
        seq_t term = binom_n11(n - (idx_t)j + 11);
        if (c > 0)
            result += term * (seq_t)c;
        else
            result -= term * (seq_t)(-c);
    }
    return result;
}
