/*
 * A240681  Number of forests with n labeled nodes and 4 trees.
 * https://oeis.org/A240681
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-8) * (n-3)*(n-2)*(n-1)*(n^3 + 21*n^2 + 202*n +
 *            840)/48
 * Range:     u8 none, u16 n <= 5, u32 n <= 12, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                /* undefined for n < 4, but not used */

    seq_t nn = (seq_t)n;

    /* Compute the polynomial n^3 + 21*n^2 + 202*n + 840 */
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t poly = n3 + (seq_t)21 * n2 + (seq_t)202 * nn + (seq_t)840;

    /* Linear factors (n-3)*(n-2)*(n-1) */
    seq_t f1 = nn - (seq_t)3;
    seq_t f2 = nn - (seq_t)2;
    seq_t f3 = nn - (seq_t)1;

    /* Multiply all factors together */
    seq_t prod = f1 * f2;   /* (n-3)*(n-2) */
    prod = prod * f3;       /* (n-3)*(n-2)*(n-1) */
    prod = prod * poly;     /* full numerator */

    /* Divide by 48 (the division is exact for n ≥ 6, truncation works for n = 4, 5) */
    seq_t div48 = prod / (seq_t)48;

    if (n >= 8) {
        /* n^(n-8) with a non‑negative exponent */
        seq_t pow = seq_pow(nn, n - 8);
        return div48 * pow;
    } else {
        /* For n < 8 we divide by n^(8-n) instead of multiplying by a negative power */
        seq_t denom = seq_pow(nn, 8 - n);
        return div48 / denom;
    }
}
