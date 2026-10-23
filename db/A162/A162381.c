/*
 * A162381  Number of reduced words of length n in the Weyl group D_34.
 * https://oeis.org/A162381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in
 *            (1-x^34)∏_{i=1}^{33}(1-x^{2i})/(1-x)^34 . * The numerator
 *            contains a factor (1-x)^34, so the result reduces to a *
 *            polynomial of degree 34^2-34 = 1122. * a(n) = coefficient of
 *            that polynomial for 0 ≤ n ≤ 1122, * and a(n) = 0 for n > 1122
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"
#include "../utils/binom.h"

/* Prime parameter for the Weyl group D_34. */
#define M       34ULL          /* degree of (1-x) in the denominator */
#define DEG_N   (M * M)        /* degree of the numerator polynomial */
#define DEG_G   (DEG_N - M)    /* degree of the final polynomial (1122) */

seq_t a(idx_t n)
{
    /* Static state – calculated only once. */
    static bool init = false;
    /* N(x) = (1-x^34)∏_{i=1}^{33}(1-x^{2i}) – integer coefficients. */
    static int64_t N_coeff[DEG_N + 1];
    /* g(x) = N(x)/(1-x)^M   – seq_t coefficients. */
    static seq_t g[DEG_G + 1];
    /* binom(M,i) for i=0..M. */
    static seq_t binom_M[ M + 1 ];

    if (!init)
    {
        /* --- 1. Build numerator coefficients N_coeff[0..DEG_N] --- */
        /* Start with polynomial 1. */
        for (idx_t i = 0; i <= DEG_N; ++i)
            N_coeff[i] = 0;
        N_coeff[0] = 1;
        int cur_deg = 0;          /* current degree of the polynomial */

        /* Exponents for the factors: 2,4,...,66 and 34. */
        idx_t exps[M];
        for (idx_t i = 0; i < M-1; ++i)
            exps[i] = 2 * (i + 1);   /* 2,4,...,66 */
        exps[M-1] = M;              /* 34 */

        for (idx_t ei = 0; ei < M; ++ei)
        {
            idx_t e = exps[ei];
            /* Multiply current polynomial by (1 - x^e). */
            for (int j = cur_deg; j >= 0; --j)
                N_coeff[j + e] -= N_coeff[j];
            cur_deg += e;           /* new degree after multiplication   */
        }

        /* --- 2. Pre‑compute binomial coefficients binom_M[i] = C(M,i) --- */
        binom_M[0] = seq_one();      /* C(M,0) = 1 */
        for (idx_t i = 1; i <= M; ++i)
            binom_M[i] = binom((idx_t)M, (idx_t)i);

        /* --- 3. Compute final polynomial coefficients g[0..DEG_G] --- */
        /* g[0] = N_coeff[0] (should be 1) */
        g[0] = (seq_t)N_coeff[0];

        /* For k from 1 to DEG_G compute g[k] via the recurrence
           arising from (1-x)^M * g(x) = N(x). */
        for (idx_t k = 1; k <= DEG_G; ++k)
        {
            seq_t pos = 0;  /* positive contributions */
            seq_t neg = 0;  /* negative contributions */

            /* Contribution from N_coeff[k] itself. */
            if (N_coeff[k] >= 0)
                pos += (seq_t)N_coeff[k];
            else
                neg += (seq_t)(-N_coeff[k]);

            /* Contributions from lower g coefficients. */
            idx_t up = (k < M) ? k : M;
            for (idx_t i = 1; i <= up; ++i)
            {
                /* term = C(M,i) * g[k-i] */
                seq_t term = seq_mul(binom_M[i], g[k - i]);

                if (i & 1)          /* odd i: sign + */
                    pos += term;
                else                /* even i: sign - */
                    neg += term;
            }

            /* g[k] = pos - neg  (guaranteed non‑negative). */
            g[k] = pos - neg;
        }

        init = true;
    }

    /* The polynomial g(x) has degree DEG_G.  For higher indices the
       coefficient is trivially 0. */
    if (n > DEG_G)
        return 0;

    return g[n];
}
