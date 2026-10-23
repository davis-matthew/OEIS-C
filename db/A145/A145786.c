/*
 * A145786  Expansion of q * chi(q^3) * chi(q^5) / (chi(q) * chi(q^15))^2 in
 *          powers of q where chi() is a Ramanujan theta function.
 * https://oeis.org/A145786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 25, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145786: expansion of
 *     q * chi(q^3) * chi(q^5) / (chi(q) * chi(q^15)^2)
 * in powers of q.  It can be written as the eta‐product
 * (η(q)η(q^4)η(q^6)η(q^10)η(q^15)η(q^60))^2 /
 * (η(q^2)^4 η(q^3) η(q^5) η(q^12) η(q^20) η(q^30)^4).
 * The coefficients a(n) for n≥1 are obtained by the recurrence
 * on a_H(k):
 *
 *   a_H(0) = 1
 *   a_H(k) = -(1/k) * Σ_{d=1}^{k} γ(d) * a_H(k-d)   for k≥1
 *   a(n)   = a_H(n-1)                                 for n≥1
 *
 * where
 *
 *   b(d)   = Σ_{m|d} e_m      (e_m are the eta exponents)
 *   γ(d)   = Σ_{t|d} t * b(t)
 *
 * The eta exponents e_m are
 *   m :  1   2   3   4   5   6  10 12 15 20 30 60
 *  e_m:  2  -4  -1   2  -1   2   2  -1   2  -1  -4   2
 *
 * The code below memoises the result in static arrays and recomputes
 * them whenever a larger n is requested.  It works for any seq_t
 * (signed 8‑32‑64‑128‑ or arbitrary‑precision) and any idx_t
 * (unsigned 64‑bit). */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

/* eta exponents */
static const idx_t  M_R[12] = { 1,  2,   3,  4,  5,   6, 10, 12, 15, 20, 30, 60 };
static const int    E_R[12] = { 2, -4, -1,  2, -1,   2,  2, -1,  2, -1, -4,   2 };

/* cached data */
static seq_t *b     = NULL;   /* b[d] = Σ_{m|d} e_m */
static seq_t *gamma = NULL;   /* γ(d) = Σ_{t|d} t * b(t) */
static seq_t *aH    = NULL;   /* coefficients a_H(k) */
static idx_t  curr_n = 0;     /* largest n already computed (for a_H) */
static size_t cap    = 0;     /* capacity of the allocated arrays */

/* ensure the static arrays are large enough and recompute all data up
 * to new_n.  The arrays are recomputed from scratch to simplify
 * incremental extensions. */
static void extend_to(idx_t new_n)
{
    /* need arrays for indices 0 .. new_n-1   (size new_n) */
    if (new_n > cap) {
        cap = new_n;
        b     = (seq_t*)realloc(b,     cap * sizeof(seq_t));
        gamma = (seq_t*)realloc(gamma, cap * sizeof(seq_t));
        aH    = (seq_t*)realloc(aH,    cap * sizeof(seq_t));
    }

    /* initialise b and gamma */
    if (new_n) {
        for (size_t i = 0; i < new_n; ++i) {
            b[i] = 0;
            gamma[i] = 0;
        }

        /* compute b[d] = Σ_{m|d} e_m */
        for (int idx = 0; idx < 12; ++idx) {
            idx_t m = M_R[idx];
            int   e = E_R[idx];
            for (size_t d = m; d < new_n; d += m) {
                b[d] += e;
            }
        }

        /* compute γ(d) = Σ_{t|d} t * b(t) */
        for (size_t t = 1; t < new_n; ++t) {
            seq_t bt = b[t];
            if (bt == 0) continue;
            for (size_t d = t; d < new_n; d += t) {
                gamma[d] += (seq_t)t * bt;
            }
        }

        /* compute a_H coefficients */
        aH[0] = (seq_t)1;
        for (size_t k = 1; k < new_n; ++k) {
            seq_t sum = 0;
            for (size_t d = 1; d <= k; ++d) {
                sum += gamma[d] * aH[k - d];
            }
            aH[k] = -(sum) / (seq_t)k;
        }
    }

    curr_n = new_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */

    /* we need a_H[n-1] => compute up to n */
    if (n > curr_n) {
        extend_to(n);
    }

    return aH[n - 1];
}
