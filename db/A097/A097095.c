/*
 * A097095  Main diagonal of triangle A097094; g.f. A(x) satisfies
 *          A(x)/(1-x-x^2) = A(x^2)^2/(1-x-x^3)^2.
 * https://oeis.org/A097095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) - a(n-6) + e(n) -
 *            e(n-1) - e(n-2), where e(2t) = Σ_{p=0..t} a(p)*a(t-p)
 *            (convolution of a with itself) and e(odd) = 0. Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=8. The function
 *            memoises computed terms to allow arbitrary call order....
 * Range:     i8 n <= 13, i16 n <= 24, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised arrays */
    static seq_t *a_arr = NULL;   /* a[0..] */
    static seq_t *e_arr = NULL;   /* e[0..] */
    static idx_t max_computed = (idx_t)-1; /* sentinel */

    /* initialise on first call */
    if (max_computed == (idx_t)-1) {
        size_t sz = (size_t)n + 1;
        a_arr = (seq_t*)malloc(sz * sizeof(seq_t));
        e_arr = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!a_arr || !e_arr) return 0;          /* allocation failure */
        a_arr[0] = 1;                            /* a(0) */
        e_arr[0] = 1;                            /* e(0) = a0*a0 */
        max_computed = 0;
    }
    /* grow arrays if necessary */
    else if (n > max_computed) {
        size_t new_sz = (size_t)n + 1;
        seq_t* new_a = (seq_t*)realloc(a_arr, new_sz * sizeof(seq_t));
        seq_t* new_e = (seq_t*)realloc(e_arr, new_sz * sizeof(seq_t));
        if (!new_a || !new_e) return a_arr[n];   /* keep previous data */
        a_arr = new_a;
        e_arr = new_e;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* compute e(i) */
        if ((i & 1) == 0) {                      /* even */
            idx_t t = i >> 1;                    /* t = i / 2 */
            seq_t sum = 0;
            for (idx_t p = 0; p <= t; ++p)
                sum += a_arr[p] * a_arr[t - p];
            e_arr[i] = sum;
        } else {
            e_arr[i] = 0;
        }

        /* recurrence for a(i) */
        seq_t val = 0;
        if (i >= 1) val += 2 * a_arr[i - 1];
        if (i >= 2) val -= a_arr[i - 2];
        if (i >= 3) val += 2 * a_arr[i - 3];
        if (i >= 4) val -= 2 * a_arr[i - 4];
        if (i >= 6) val -= a_arr[i - 6];
        val += e_arr[i];
        if (i >= 1) val -= e_arr[i - 1];
        if (i >= 2) val -= e_arr[i - 2];
        a_arr[i] = val;
    }

    if (n > max_computed)
        max_computed = n;

    return a_arr[n];
}
