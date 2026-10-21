/*
 * A082689  Numerator of n*sum(k=1,(-1)^(k+1)/(n+k)).
 * https://oeis.org/A082689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of n * sum_{k=1}^n (-1)^(k+1) / (n + k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_seq.h"

/* Compute gcd of two seq_t values */

seq_t a(idx_t n)
{
    seq_t num = 0;
    seq_t den = 1;
    seq_t nseq = (seq_t)n;
    idx_t m = n / 2;  /* number of positive-negative pairs */

    for (idx_t i = 1; i <= m; ++i) {
        seq_t d1 = (seq_t)(n + 2 * i - 1);
        seq_t d2 = (seq_t)(n + 2 * i);
        seq_t denom = d1 * d2;          /* 1 / (d1*d2) */

        /* Add 1/denom to the current fraction num/den */
        seq_t g  = gcd_seq(den, denom);
        seq_t den_div   = den / g;
        seq_t denom_div = denom / g;

        seq_t new_num = num * denom_div + den_div;
        seq_t new_den = den_div * denom;

        seq_t g2 = gcd_seq(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    /* If n is odd, add the remaining term 1/(2n) */
    if (n % 2 == 1) {
        seq_t denom = (seq_t)(2 * n);
        seq_t g = gcd_seq(den, denom);
        seq_t den_div   = den / g;
        seq_t denom_div = denom / g;

        seq_t new_num = num * denom_div + den_div;
        seq_t new_den = den_div * denom;

        seq_t g2 = gcd_seq(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    /* Multiply the fraction by n and simplify once more */
    seq_t g_n   = gcd_seq(nseq, den);
    seq_t n_adj = nseq / g_n;
    seq_t den_f = den / g_n;

    seq_t num_f = num * n_adj;   /* num_f and den_f are coprime now */

    return num_f;
}
