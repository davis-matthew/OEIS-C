/*
 * A128591  Expansion of f(x, x^5) * f(x, x^3) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A128591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A128591: coefficients of f(x,x^5)*f(x,x^3) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* we compute the coefficient of x^n in
         f(x,x^5)*f(x,x^3)
       where
         f(a,b) = sum_{k=-∞}^{∞} a^{k(k+1)/2} b^{k(k-1)/2}
       The two series have the form
         sum_{m>=0}        x^{3m^2-2m}       (m=0 gives 1)
         + sum_{m>=1}      x^{3m^2+2m}
       and
         sum_{m>=0}        x^{2m^2-m}        (m=0 gives 1)
         + sum_{m>=1}      x^{2m^2+m};
       Each term has coefficient 1, so the product coefficient is
       simply the number of pairs (e1,e2) with e1 in the first list,
       e2 in the second list, and e1+e2=n.
    */
    size_t N = (size_t)n;        /* local 64‑bit unsigned copy of the index */

    /* ------------------------------------------------------------------ */
    /* build the list of exponents of the first theta function up to N   */
    /* ------------------------------------------------------------------ */
    size_t      e1cnt = 0, e2cnt = 0;
    /* a fixed upper bound that is comfortably larger than sqrt(N)+10  */
    /* for N <= 10000 this is already more than enough.               */
    size_t      cap   = 256;     /* more than 150 entries will suffice   */
    int        *e1    = (int*)malloc(cap * sizeof(int));
    int        *e2    = (int*)malloc(cap * sizeof(int));

    /*  term for m = 0 is always 0 and belongs to both lists         */
    e1[e1cnt++] = 0;
    e2[e2cnt++] = 0;

    /*  add the positive and negative parts for m >= 1                */
    for (size_t m = 1;; ++m) {
        int pos = (int)(3 * m * m - 2 * m);
        int neg = (int)(3 * m * m + 2 * m);
        if (pos <= (int)N) {
            if (e1cnt == cap) { cap *= 2; e1 = (int*)realloc(e1, cap * sizeof(int)); }
            e1[e1cnt++] = pos;
        }
        if (neg <= (int)N) {
            if (e1cnt == cap) { cap *= 2; e1 = (int*)realloc(e1, cap * sizeof(int)); }
            e1[e1cnt++] = neg;
        }
        if (pos > (int)N && neg > (int)N) break;
    }

    /* ------------------------------------------------------------------ */
    /* build the list of exponents of the second theta function up to N */
    /* ------------------------------------------------------------------ */
    cap = 256;
    free(e2);
    e2   = (int*)malloc(cap * sizeof(int));
    e2cnt = 0;
    e2[e2cnt++] = 0;
    for (size_t m = 1;; ++m) {
        int pos = (int)(2 * m * m - m);
        int neg = (int)(2 * m * m + m);
        if (pos <= (int)N) {
            if (e2cnt == cap) { cap *= 2; e2 = (int*)realloc(e2, cap * sizeof(int)); }
            e2[e2cnt++] = pos;
        }
        if (neg <= (int)N) {
            if (e2cnt == cap) { cap *= 2; e2 = (int*)realloc(e2, cap * sizeof(int)); }
            e2[e2cnt++] = neg;
        }
        if (pos > (int)N && neg > (int)N) break;
    }

    /* ------------------------------------------------------------------ */
    /* convolution: number of pairs (e1,e2) with sum == N                */
    /* ------------------------------------------------------------------ */
    seq_t result = 0;
    for (size_t i = 0; i < e1cnt; ++i) {
        int e1v = e1[i];
        for (size_t j = 0; j < e2cnt; ++j) {
            size_t sum = (size_t)e1v + (size_t)e2[j];
            if (sum == N) result++;
        }
    }

    /* ------------------------------------------------------------------ */
    /* clean up ---------------------------------------------------------- */
    /* ------------------------------------------------------------------ */
    free(e1);
    free(e2);

    return result;
}
