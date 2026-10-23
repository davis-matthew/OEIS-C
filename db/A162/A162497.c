/*
 * A162497  Number of reduced words of length n in the reflection group
 *          [3,3,5] of order 14400.
 * https://oeis.org/A162497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = Σ c[i] * binomial(n-i+3,3), where c[0]=1, c[2]=-1,
 *            c[12]=-1, c[14]=1, c[20]=-1, c[22]=1, c[30]=-1, c[32]=2,
 *            c[34]=-1, c[42]=1, c[44]=-1, c[50]=1, c[52]=-1, c[62]=-1,
 *            c[64]=1, all other c[i]=0
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum is taken over 0 ≤ i ≤ min(n,64). */
#include <stdint.h>
#include <stddef.h>

/* coefficients of the numerator polynomial (1−x²)(1−x¹²)(1−x²⁰)(1−x³⁰) */
static const int8_t C[65] = {
    /*  0 */  1,  /*  1 */  0,  /*  2 */ -1,  /*  3 */  0,  /*  4 */  0,
    /*  5 */  0,  /*  6 */  0,  /*  7 */  0,  /*  8 */  0,  /*  9 */  0,
    /* 10 */  0,  /* 11 */  0,  /* 12 */ -1,  /* 13 */  0,  /* 14 */  1,
    /* 15 */  0,  /* 16 */  0,  /* 17 */  0,  /* 18 */  0,  /* 19 */  0,
    /* 20 */ -1,  /* 21 */  0,  /* 22 */  1,  /* 23 */  0,  /* 24 */  0,
    /* 25 */  0,  /* 26 */  0,  /* 27 */  0,  /* 28 */  0,  /* 29 */  0,
    /* 30 */ -1,  /* 31 */  0,  /* 32 */  2,  /* 33 */  0,  /* 34 */ -1,
    /* 35 */  0,  /* 36 */  0,  /* 37 */  0,  /* 38 */  0,  /* 39 */  0,
    /* 40 */  0,  /* 41 */  0,  /* 42 */  1,  /* 43 */  0,  /* 44 */ -1,
    /* 45 */  0,  /* 46 */  0,  /* 47 */  0,  /* 48 */  0,  /* 49 */  0,
    /* 50 */  1,  /* 51 */  0,  /* 52 */ -1,  /* 53 */  0,  /* 54 */  0,
    /* 55 */  0,  /* 56 */  0,  /* 57 */  0,  /* 58 */  0,  /* 59 */  0,
    /* 60 */  0,  /* 61 */  0,  /* 62 */ -1,  /* 63 */  0,  /* 64 */  1
};

/* compute binomial (m,3) where m = n-i+3, m>=3 */
static inline seq_t binom3(idx_t m)
{
    seq_t a = (seq_t)m;
    seq_t b = (seq_t)(m - 1);
    seq_t c = (seq_t)(m - 2);
    return (a * b * c) / 6;
}

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* Only i up to 64 and not exceeding n */
    idx_t max_i = n < 64 ? n : 64;
    for (idx_t i = 0; i <= max_i; ++i) {
        int8_t coeff = C[i];
        if (coeff == 0) continue;

        seq_t term = binom3(n - i + 3); /* >=0 because i <= n */

        /* handle coefficients 1, -1, 2, -2 */
        if (coeff > 0)
            sum_pos += term * (seq_t)coeff;
        else
            sum_neg += term * (seq_t)(-coeff);
    }

    return sum_pos - sum_neg;
}
