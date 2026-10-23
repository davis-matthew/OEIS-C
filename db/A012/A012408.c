/*
 * A012408  exp(arctan(x)*exp(x))=1+x+3/2!*x^2+8/3!*x^3+25/4!*x^4+100/5!*x^5...
 * https://oeis.org/A012408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) as the integer coefficient of x^n in exp(arctan(x)*exp(x)),
 *            i.e. a(n) = n! * [x^n] exp(arctan(x)*exp(x))
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   compute_P
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence follows from
 *     a(0)=1,
 *     a(n) = Σ_{k=1..n} P(k) * C(n-1,k-1) * a(n-k),
 * where P(k) = k! * [x^k] (arctan(x)*exp(x)) is an integer.
 * P(k) can be written in a closed form:
 *     P(k) = Σ_{s odd, 1≤s≤k} (-1)^((s-1)/2) * (k!/(k-s)!)/s .
 *
 * All arithmetic is performed with type seq_t (signed integer, maybe
 * arbitrary‑precision). No floating point or math library is used.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_P.h"

/* --------------------------------------------------------------------- */
/* compute P(k) = k! * coefficient of x^k in arctan(x)*exp(x)             */
/* the result is integer                                           */

/* --------------------------------------------------------------------- */
/* memoised sequence a(n)                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *a_tbl = NULL;    /* a[0..max] */
    static seq_t *P_tbl = NULL;    /* P[0..max] (P[0] unused) */
    static idx_t known = (idx_t)-1; /* largest n computed */

    if (known == (idx_t)-1) {      /* first call */
        known = 0;
        a_tbl = (seq_t*)malloc(sizeof(seq_t));
        P_tbl = (seq_t*)malloc(sizeof(seq_t));
        a_tbl[0] = 1;             /* a(0) = 1 */
        P_tbl[0] = 0;             /* dummy */
    }

    if (n > known) {
        size_t new_cap = (size_t)n + 1U;

        a_tbl = (seq_t*)realloc(a_tbl, new_cap * sizeof(seq_t));
        P_tbl = (seq_t*)realloc(P_tbl, new_cap * sizeof(seq_t));

        /* compute missing entries */
        for (idx_t i = known + 1U; i <= n; ++i) {
            /* compute P[i] */
            P_tbl[i] = compute_P(i);

            /* compute a[i] via recurrence */
            seq_t total = 0;
            seq_t binom = 1;     /* C(i-1,0) */
            for (idx_t k = 1U; k <= i; ++k) {
                seq_t term = binom * P_tbl[k];
                term *= a_tbl[i - k];
                total += term;

                /* update binom to C(i-1, k) for next iteration */
                if (k < i) {
                    /* binom = C(i-1, k) = binom * (i-1 - (k-1)) / k */
                    binom *= (seq_t)(i - k);
                    binom /= (seq_t)(k);
                }
            }
            a_tbl[i] = total;
        }
        known = n;
    }
    return a_tbl[n];
}
