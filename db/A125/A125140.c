/*
 * A125140  SEPSigma(n) = (-1)^(Sum_i r_i)*Sum_{d|n} (-1)^(Sum_j Max(r_j))*d =
 *          Product_i (Sum_{s_i=1..r_i} p_i^s_i)+(-1)^r_i where n = Product_i
 *          p_i^r_i, d = Product_j p_j^r_j, p_j^max(r_j) is the largest power
 *          of p_j dividing n.
 * https://oeis.org/A125140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product_i [(-1)^{r_i} + (p_i^{\,r_i+1} - p_i)/(p_i-1)]
 * Range:     u8 n <= 16, u16 n <= 3872, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n = Π p_i^{r_i} is the prime factorisation of n */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle n == 1 (empty product) */
    if (n == 1)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t tmp = n;

    /* factor 2 separately */
    int e = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t pow_val = (seq_t)1;
        for (int i = 0; i < e + 1; ++i)
            pow_val *= (seq_t)2;                      /* 2^(e+1) */
        seq_t termAdd = (pow_val - (seq_t)2) / ((seq_t)2 - 1); /* (2^(e+1)-2)/(2-1) */
        seq_t factor = (e & 1) ? (termAdd - (seq_t)1) : (termAdd + (seq_t)1);
        prod *= factor;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t pow_val = (seq_t)1;
            for (int i = 0; i < exp + 1; ++i)
                pow_val *= (seq_t)p;                      /* p^(exp+1) */
            seq_t termAdd = (pow_val - (seq_t)p) / ((seq_t)p - 1); /* (p^(exp+1)-p)/(p-1) */
            seq_t factor = (exp & 1) ? (termAdd - (seq_t)1) : (termAdd + (seq_t)1);
            prod *= factor;
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;
        /* exponent is 1 */
        seq_t pow_val = p * p;                     /* p^(1+1) */
        seq_t termAdd = (pow_val - p) / (p - 1);   /* (p^2-p)/(p-1) = p */
        seq_t factor = (termAdd - (seq_t)1);        /* exp=1 is odd */
        prod *= factor;
    }

    return prod;
}
