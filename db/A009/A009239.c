/*
 * A009239  Expansion of e.g.f.: exp(tan(sinh(x))).
 * https://oeis.org/A009239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n/n! in exp(tan(sinh(x)))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let T(x)=tan(sinh x)=∑_{k≥1} t_k x^k/k!,
   C(x)=cosh x=∑_{k≥0} c_k x^k/k! (c_k=1 if k even else 0),
   S(x)=1+T(x)^2=∑_{k≥0} s_k x^k/k!.

   Then for all n≥0:  t_{n+1}=∑_{k=0}^n binom(n,k) c_k s_{n-k},
                       s_n   =∑_{k=0}^n binom(n,k) t_k t_{n-k}.
   For the e.g.f. A(x)=exp(T(x))=∑_{n≥0} a_n x^n/n! we have
   a_0=1 and for n≥1:
          a_n = ∑_{k=1}^n binom(n-1,k-1) t_k a_{n-k} .
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for t, s and a. */
    size_t N = (size_t)n;
    seq_t *t = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *s = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!t || !s || !arr) {
        /* In the unlikely event of allocation failure, return 0. */
        return (seq_t)0;
    }

    /* Initial values. */
    t[0] = (seq_t)0;  /* T(0)=0 */
    s[0] = (seq_t)1;  /* 1+T(0)^2 = 1 */
    arr[0] = (seq_t)1; /* A(0)=1 */

    /* Compute t[1..n] and s[1..n]. */
    for (idx_t i = 1; i <= n; ++i) {
        /* t[i] = t_i (coeff of x^i/i!) , using recurrence with i-1. */
        seq_t sum_t = (seq_t)0;
        seq_t bin = (seq_t)1; /* binom(i-1,0) */
        for (idx_t k = 0; k <= i - 1; ++k) {
            if ((k & 1) == 0) /* c_k = 1 when k even */
                sum_t += bin * s[(idx_t)(i - 1 - k)];
            if (k < i - 1) {
                /* Update binom(i-1,k+1) = binom(i-1,k)*(i-1-k)/(k+1) */
                bin = bin * (seq_t)((i - 1) - k) / (seq_t)(k + 1);
            }
        }
        t[i] = sum_t;

        /* s[i] = sum_{k=0}^i binom(i,k) t_k t_{i-k}. */
        seq_t sum_s = (seq_t)0;
        bin = (seq_t)1; /* binom(i,0) */
        for (idx_t k = 0; k <= i; ++k) {
            sum_s += bin * t[k] * t[(idx_t)(i - k)];
            if (k < i) {
                /* Update binom(i,k+1) = binom(i,k)*(i-k)/(k+1) */
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        s[i] = sum_s;
    }

    /* Compute a[1..n]. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum_a = (seq_t)0;
        seq_t bin = (seq_t)1; /* binom(i-1,0) for k=1 */
        for (idx_t k = 1; k <= i; ++k) {
            sum_a += bin * t[k] * arr[(idx_t)(i - k)];
            if (k < i) {
                /* binom(i-1,k) = binom(i-1,k-1)*(i-1-(k-1))/k = bin*(i-k)/k */
                bin = bin * (seq_t)(i - k) / (seq_t)k;
            }
        }
        arr[i] = sum_a;
    }

    seq_t result = arr[n];

    /* Free allocated memory. */
    free(t);
    free(s);
    free(arr);

    return result;
}
