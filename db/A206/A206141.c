/*
 * A206141  G.f.: Sum_{n>=0} x^n/Product_{k=1..n} (1 - A002203(k)*x^k +
 *          (-1)^k*x^(2*k)), where A002203 is the companion Pell numbers.
 * https://oeis.org/A206141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206141
 *
 * A(x) = Σ_{n≥0} x^n / Π_{k=1..n} (1 - A002203(k)*x^k + (-1)^k*x^(2k))
 * with A002203(1)=2, A002203(2)=6, A002203(n)=2*A002203(n-1)+A002203(n-2)
 *
 * Recurrence used:
 * Let B_k(x)=1/Π_{j=1..k} (1 - A002203(j)*x^j + (-1)^j*x^(2j)).
 * B_0(x)=1.
 * For k≥1:
 *   B_k(x) * (1 - a_k*x^k + (-1)^k*x^(2k)) = B_{k-1}(x)
 *   with a_k=A002203(k).
 * Coefficients c_m of B_k satisfy:
 *   c_m = f_m                               , m<k
 *   c_m = f_m + a_k * c_{m-k}                , k≤m<2k
 *   c_m = f_m + a_k * c_{m-k} ± c_{m-2k}      , m≥2k
 *     (plus if (-1)^k=-1, minus if (-1)^k=+1)
 *
 * The desired sequence a(n) is the coefficient of x^n in A(x)
 *        a(n)=Σ_{k=0..n} [x^{n-k}]B_k(x)
 *
 * The algorithm computes B_k iteratively, adds the contribution
 * from each k to the answer array, and finally returns a(n).
 */

#include <stdint.h>
#include <stdlib.h>

/* Function definition required by the build */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    /* Convert n to size_t for array sizes */
    size_t N = (size_t)n;

    /* Allocate coefficient arrays */
    seq_t *coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));      /* B_{k-1} */
    seq_t *newcoeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* B_k     */
    seq_t *ans = (seq_t *)malloc((N + 1) * sizeof(seq_t));        /* a[n]    */
    if (!coeff || !newcoeff || !ans) {
        /* Allocation failure – return 0 (unlikely for normal use) */
        free(coeff);
        free(newcoeff);
        free(ans);
        return (seq_t)0;
    }

    /* Initialize B_0(x)=1 */
    for (size_t i = 0; i <= N; ++i) {
        coeff[i] = (seq_t)0;
        newcoeff[i] = (seq_t)0;
        ans[i] = (seq_t)0;
    }
    coeff[0] = (seq_t)1;
    ans[0] = (seq_t)1;     /* contribution of the k=0 term */

    /* Companion Pell numbers for A002203 (2,6,14,34,...) */
    seq_t *pell = (seq_t *)malloc((N + 2) * sizeof(seq_t)); /* use N+2 to avoid underflow */
    if (!pell) {
        free(coeff);
        free(newcoeff);
        free(ans);
        return (seq_t)0;
    }
    pell[0] = (seq_t)0;  /* unused */
    pell[1] = (seq_t)2;
    if (N >= 2) pell[2] = (seq_t)6;
    for (size_t k = 3; k <= N; ++k) {
        pell[k] = (seq_t)2 * pell[k - 1] + pell[k - 2];
    }

    /* Main loop over k */
    for (size_t k = 1; k <= N; ++k) {
        seq_t a_k = pell[k];
        int sign_bk = (k % 2 == 0) ? 1 : -1;   /* (-1)^k */

        /* Compute B_k coefficients from B_{k-1} */
        for (size_t m = 0; m <= N; ++m) {
            if (m < k) {
                newcoeff[m] = coeff[m];
            } else if (m < 2 * k) {
                /* g_m = f_m + a_k * g_{m-k} */
                newcoeff[m] = coeff[m] + a_k * newcoeff[m - k];
            } else {
                /* g_m = f_m + a_k * g_{m-k} ± g_{m-2k} */
                seq_t term = coeff[m] + a_k * newcoeff[m - k];
                if (sign_bk == 1)  /* (-1)^k = +1 -> minus */
                    newcoeff[m] = term - newcoeff[m - 2 * k];
                else               /* (-1)^k = -1 -> plus */
                    newcoeff[m] = term + newcoeff[m - 2 * k];
            }
        }

        /* Add contribution of term k to a[] */
        for (size_t m = k; m <= N; ++m) {
            ans[m] += newcoeff[m - k];
        }

        /* Prepare for next iteration */
        seq_t *tmp = coeff;
        coeff = newcoeff;
        newcoeff = tmp;
    }

    seq_t result = ans[N];

    /* Clean up */
    free(coeff);
    free(newcoeff);
    free(ans);
    free(pell);

    return result;
}
