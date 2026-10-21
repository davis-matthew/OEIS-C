/*
 * A229807  G.f. satisfies: A(x) = x*exp( Sum_{n>=1} A(sigma(n)*x^n) / n ).
 * https://oeis.org/A229807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = x * exp( Sum_{n>=1} A(sigma(n)*x^n) / n ) Recurrence from
 *            the power‑series expansion is for n>=1: (n) * e_n = Σ_{k=1}^{n}
 *            c_k * e_{n-k} where e_0 = 1 , a_{n+1} = e_n c_k = Σ_{d|k} a_d *
 *            sigma(k/d)^d * d All computations use exact integer arithmetic;
 *            the intermediate coefficients c_k are in...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* static workspace -------------------------------------------------------- */
static seq_t *A = NULL;      /* a[1..max_n]          */
static seq_t *C = NULL;      /* c[1..max_n]          */
static seq_t *E = NULL;      /* e[0..max_n-1]        */
static seq_t *sigma = NULL;  /* sigma[1..max_n]      */
static idx_t capacity = 0;   /* highest n precomputed */

/* ------------------------------------------------------------------------ */
static void ensure_workspace(idx_t n)
{
    if (n <= capacity) return;

    /* reallocate arrays */
    A      = (seq_t *)realloc(A,     (n + 2) * sizeof(seq_t));
    C      = (seq_t *)realloc(C,     (n + 1) * sizeof(seq_t));
    E      = (seq_t *)realloc(E,     (n + 1) * sizeof(seq_t));
    sigma  = (seq_t *)realloc(sigma,  (n + 1) * sizeof(seq_t));

    /* compute divisor sum sigma(1..n) */
    for (idx_t i = 1; i <= n; ++i) sigma[i] = 0;
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += (seq_t)i;

    /* initialise base cases ------------------------------------------------- */
    A[1] = 1;              /* a(1) = 1  */
    C[1] = sigma[1] * (seq_t)1;  /* c1 = 1*1*1 = 1 */
    E[0] = 1;              /* e0 = 1    */

    /* compute remaining terms up to n ------------------------------------- */
    for (idx_t m = 1; m < n; ++m)
    {
        /* 1. compute e_m = (1/m) * Σ_{k=1}^{m} c_k * e_{m-k} */
        seq_t sum_e = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum_e += C[k] * E[m - k];
        E[m] = sum_e / (seq_t)m;       /* division is exact */

        /* 2. set a_{m+1} = e_m */
        A[m + 1] = E[m];

        /* 3. compute c_{m+1} = Σ_{d|m+1} a_d * sigma((m+1)/d)^d * d */
        idx_t kp = m + 1;
        seq_t c_val = 0;
        for (idx_t d = 1; d * d <= kp; ++d)
        {
            if (kp % d == 0)
            {
                /* divisor d */
                idx_t n1 = kp / d;
                seq_t pow1 = big_pow((seq_t)sigma[n1], (seq_t)d);
                seq_t term1 = pow1 * A[d] * (seq_t)d;
                c_val += term1;

                /* complementary divisor n1 if distinct */
                if (d != n1)
                {
                    idx_t d2 = n1;
                    idx_t n2 = d;
                    seq_t pow2 = big_pow((seq_t)sigma[n2], (seq_t)d2);
                    seq_t term2 = pow2 * A[d2] * (seq_t)d2;
                    c_val += term2;
                }
            }
        }
        C[kp] = c_val;
    }

    capacity = n;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, safe guard */
    ensure_workspace(n);
    return A[n];
}
