/*
 * A199629  G.f.:
 *          (1+x)^(2*g)*(1+x^3)^(3*g)/((1-x^2)*(1-x^4))-x^(2*g)*(1+x)^4/((1-x^2)*(1-x^4))
 *          for g=3.
 * https://oeis.org/A199629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = a(n-2) + a(n-4) - a(n-6) + b_n * where b_n is the
 *            coefficient of x^n in * (1+x)^6·(1+x^3)^9 – x^6·(1+x)^4. * b_n =
 *            0 for n > 33. * Initial values: a(0)=1, a(1)=6, a(2)=16, a(3)=35
 *            … as given
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* -------------------------------------------------------------------- */
/* Helper: binomial coefficient (small n, k ≤ 9)                        */
/* -------------------------------------------------------------------- */
static seq_t binom_small(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* -------------------------------------------------------------------- */
/* Compute the 34 coefficients b[0 … 33] once.                           */
/* -------------------------------------------------------------------- */
static void init_b_coeff(seq_t *b)
{
    for (int i = 0; i < 34; ++i) b[i] = 0;

    /* (1+x)^6 * (1+x^3)^9 */
    for (int i = 0; i <= 6; ++i) {
        seq_t ci = binom_small(6, i);
        for (int j = 0; j <= 9; ++j) {
            seq_t cj = binom_small(9, j);
            int deg = i + 3 * j;
            b[deg] = b[deg] + ci * cj;
        }
    }

    /* subtract x^6 * (1+x)^4 */
    for (int l = 0; l <= 4; ++l) {
        seq_t coef = binom_small(4, l);
        int deg = 6 + l;
        b[deg] = b[deg] - coef;
    }
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool b_inited = false;
    static seq_t b[34];

    if (!b_inited) {
        init_b_coeff(b);
        b_inited = true;
    }

    /* handle the very small indices directly */
    if (n == 0) return b[0];   /* 1 */
    if (n == 1) return b[1];   /* 6 */

    /* even indices: a[2k] */
    if ((n & 1U) == 0U) {
        idx_t k = n >> 1;           /* k = n/2 */

        /* base cases */
        if (k == 0) return b[0];
        if (k == 1) return b[0] + b[2];
        if (k == 2) return (b[0] + b[2]) + b[0] + b[4];

        seq_t a0 = b[0];            /* a[0]   */
        seq_t a2 = a0 + b[2];       /* a[2]   */
        seq_t a4 = a2 + a0 + b[4];  /* a[4]   */

        seq_t prev2 = a0;  /* a[0] */
        seq_t prev1 = a2;  /* a[2] */
        seq_t curr  = a4;  /* a[4] */

        for (idx_t kk = 3; kk <= k; ++kk) {
            seq_t bcoef = 0;
            idx_t idx = kk * 2;
            if (idx <= 33) bcoef = b[idx];
            seq_t anew = curr + prev1 - prev2 + bcoef;
            prev2 = prev1;
            prev1 = curr;
            curr  = anew;
        }
        return curr;
    }

    /* odd indices: a[2k+1] */
    {
        idx_t k = (n - 1) >> 1;   /* k = (n-1)/2 */

        /* base cases */
        if (k == 0) return b[1];
        if (k == 1) return b[1] + b[3];
        if (k == 2) return (b[1] + b[3]) + b[1] + b[5];

        seq_t a1 = b[1];            /* a[1]   */
        seq_t a3 = a1 + b[3];       /* a[3]   */
        seq_t a5 = a3 + a1 + b[5];  /* a[5]   */

        seq_t prev2 = a1;  /* a[1] */
        seq_t prev1 = a3;  /* a[3] */
        seq_t curr  = a5;  /* a[5] */

        for (idx_t kk = 3; kk <= k; ++kk) {
            seq_t bcoef = 0;
            idx_t idx = kk * 2 + 1;
            if (idx <= 33) bcoef = b[idx];
            seq_t anew = curr + prev1 - prev2 + bcoef;
            prev2 = prev1;
            prev1 = curr;
            curr  = anew;
        }
        return curr;
    }
}
