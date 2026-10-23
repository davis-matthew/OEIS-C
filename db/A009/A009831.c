/*
 * A009831  Expansion of e.g.f. tanh(x)*exp(tanh(x)).
 * https://oeis.org/A009831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n/n! in tanh(x)*exp(tanh(x))
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements the recurrence:
 *    t[0] = 0,  t[1] = 1,  t[n] = – Σ_{k=0}^{n-1} C(n-1,k) t[k] t[n-1-k]      (n ≥ 2)
 *    e[0] = 1,  e[n] =   Σ_{k=0}^{n-1} C(n-1,k) e[k] t[n-1-k]      (n ≥ 1)
 *    a[n] =   Σ_{k=0}^{n}   C(n,k)   t[k] e[n-k]                    (n ≥ 0)
 *
 *  The function a(idx_t n) returns a(n) for any non‑negative n, independent
 *  of call order, and runs in O(n²) time with O(n) auxiliary storage.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* idx_t is provided by the build system */
#include <stddef.h>   /* size_t */

/* Grow or allocate a dynamic array of seq_t values. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 1 : (*cap << 1);
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *arr  = tmp;
    *cap  = newcap;
}

/* Return the nth term of A009831. */
seq_t a(idx_t n)
{
    static seq_t *t = NULL;         /* coefficients of tanh */
    static seq_t *e = NULL;         /* coefficients of exp(tanh) */
    static seq_t *f = NULL;         /* coefficients of the product */
    static size_t tcap = 0, ecap = 0, fcap = 0;
    static size_t cur_max = 0;       /* highest index already computed */

    /* Initialisation for index 0. */
    if (cur_max == 0 && t == NULL) {
        ensure_capacity(&t,  &tcap, 1);
        ensure_capacity(&e,  &ecap, 1);
        ensure_capacity(&f,  &fcap, 1);
        t[0] = (seq_t)0;
        e[0] = (seq_t)1;
        f[0] = (seq_t)0;
        cur_max = 0;
    }

    /* Extend precomputation up to n if necessary. */
    if (n > (idx_t)cur_max) {
        size_t new_max = (size_t)n;
        ensure_capacity(&t, &tcap, new_max + 1);
        ensure_capacity(&e, &ecap, new_max + 1);
        ensure_capacity(&f, &fcap, new_max + 1);

        for (size_t i = cur_max + 1; i <= new_max; ++i) {
            /* ---- compute t[i] ---- */
            if (i == 1) {
                t[i] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                size_t j = i - 1;             /* j = n-1 in recurrence */
                seq_t comb = (seq_t)1;        /* C(j,0) */
                for (size_t k = 0; k <= j; ++k) {
                    seq_t term = t[k] * t[j - k];
                    sum += comb * term;
                    if (k < j)
                        comb = comb * (seq_t)(j - k) / (seq_t)(k + 1);  /* C(j,k+1) */
                }
                t[i] = -sum;
            }

            /* ---- compute e[i] ---- */
            if (i == 0) {
                e[i] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                size_t j = i - 1;
                seq_t comb = (seq_t)1;
                for (size_t k = 0; k <= j; ++k) {
                    seq_t term = e[k] * t[i - k];
                    sum += comb * term;
                    if (k < j)
                        comb = comb * (seq_t)(j - k) / (seq_t)(k + 1);
                }
                e[i] = sum;
            }

            /* ---- compute f[i] ---- */
            if (i == 0) {
                f[i] = (seq_t)0;
            } else {
                seq_t sum = (seq_t)0;
                seq_t comb = (seq_t)1;
                for (size_t k = 0; k <= i; ++k) {
                    seq_t term = t[k] * e[i - k];
                    sum += comb * term;
                    if (k < i)
                        comb = comb * (seq_t)(i - k) / (seq_t)(k + 1);
                }
                f[i] = sum;
            }
        }
        cur_max = new_max;
    }

    return f[(size_t)n];
}
