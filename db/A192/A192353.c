/*
 * A192353  Constant term of the reduction of the polynomial
 *          p(n,x)=(1/2)((x+2)^n+(x-2)^n) by x^2->x+1.
 * https://oeis.org/A192353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = constant term of ((x+2)^{n-1}+(x-2)^{n-1})/2 reduced by
 *            x²→x+1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined in the build environment */
seq_t a(idx_t n)
{
    /* OEIS offset is 1, so a(1) corresponds to n‑1 = 0 */
    idx_t m = n - 1;                /* exponent in (x±2)^m */
    /* Pre‑compute Fibonacci numbers up to m‑1 (B_k = F_{k-1}), needed for the
       constant part of x^k after reduction */
    seq_t *fib = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!fib)
        return (seq_t)0;           /* allocation failure – should not happen */

    fib[0] = (seq_t)0;
    if (m >= 1)
        fib[1] = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* Sum over k = m, m-2, …, 0 where n‑k is even */
    seq_t sum   = 0;
    seq_t binom = 1;                /* C(m, m) */
    seq_t pow2  = 1;                /* 2^0 */
    idx_t  k    = m;
    while (1) {
        seq_t Bk = (k == 0) ? (seq_t)1 : fib[k - 1];
        seq_t term = binom;
        term *= pow2;
        term *= Bk;
        sum += term;

        if (k < 2)
            break;                 /* no further terms */

        /* Update binom = C(m, k-2) from C(m, k) */
        seq_t num = binom;
        num *= (seq_t)k;
        num *= (seq_t)(k - 1);
        seq_t den = (seq_t)(m - k + 1);
        den *= (seq_t)(m - k + 2);
        binom = num / den;        /* exact division */

        pow2 *= (seq_t)4;          /* 2^(2 * (m-k/2)) increases by factor 4 each step */
        k -= 2;
    }

    free(fib);
    return sum;
}
