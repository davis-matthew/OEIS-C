/*
 * A201951  G.f.: Sum_{n>=0} x^n * Product_{k=0..n-1} (1 + k*x + x^2).
 * https://oeis.org/A201951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * ∑_{m≥0} x^m · ∏_{k=0}^{m‑1} (1 +
 *            k·x + x²) * (OEIS A201951) – computed by building the polynomial
 *            * iteratively. The algorithm keeps the polynomial of degree ≤ n
 *            * and sums the coefficient of xⁿ from each intermediate product
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 35, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C/C++ dual compile */
#if defined(__cplusplus)
#include <cstddef>
#else
#include <stddef.h>
#endif
#include <stdlib.h>

/* In C++ the type seq_t is an arbitrary‑precision integer class,
 * in C it is a plain unsigned integer type (e.g. uint64_t).  */

seq_t a(idx_t n)
{
    /* n = 0 gives the first term. */
    if (n == 0)
        return (seq_t)1;

    const size_t max_len = (size_t)n + 1;          /* degree ≤ n */

#ifdef __cplusplus
    /* C++: allocate arrays with constructors/destructors. */
    seq_t *P   = new seq_t[max_len];
    seq_t *Tmp = new seq_t[max_len];
#else
    /* C: simple malloc, no constructors needed. */
    seq_t *P   = (seq_t*)malloc(max_len * sizeof(seq_t));
    seq_t *Tmp = (seq_t*)malloc(max_len * sizeof(seq_t));
#endif
    if (!P || !Tmp) {                      /* allocation failed */
#ifdef __cplusplus
        delete[] P; delete[] Tmp;
#else
        free(P); free(Tmp);
#endif
        return (seq_t)0;
    }

    /* initialise polynomial 1. */
    for (size_t i = 0; i < max_len; ++i)
        P[i] = (seq_t)0;
    P[0] = (seq_t)1;

    size_t cur_len = 1;     /* current number of non‑zero coefficients */
    seq_t total = (seq_t)0;

    /* iterate over m = 0 … n */
    for (idx_t m = 0; m <= n; ++m) {
        idx_t j = n - m;                       /* coefficient needed from P_m */
        if ((size_t)j < cur_len)
            total += P[j];

        if (m == n) break;                     /* last term – no more product */

        /* new length after multiplying by (1 + m·x + x²): add 2 but cap at max_len */
        size_t new_len = cur_len + 2;
        if (new_len > max_len) new_len = max_len;

        /* zero Tmp up to new_len (no need to touch beyond). */
        for (size_t i = 0; i < new_len; ++i)
            Tmp[i] = (seq_t)0;

        /* multiply current polynomial by (1 + m·x + x²) */
        for (size_t i = 0; i < cur_len; ++i) {
            /* term with coefficient 1 (no x) */
            if (i < new_len)
                Tmp[i] += P[i];

            /* term with coefficient m (x¹) */
            if (i + 1 < new_len)
                Tmp[i + 1] += P[i] * (seq_t)m;

            /* term with coefficient 1 (x²) */
            if (i + 2 < new_len)
                Tmp[i + 2] += P[i];
        }

        /* swap and update current length */
        seq_t *swap = P;
        P = Tmp;
        Tmp = swap;
        cur_len = new_len;
    }

    /* free allocated memory */
#ifdef __cplusplus
    delete[] P;
    delete[] Tmp;
#else
    free(P);
    free(Tmp);
#endif
    return total;
}
