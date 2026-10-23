/*
 * A009184  Expansion of e.g.f.: cosh(x)/exp(sin(x)).
 * https://oeis.org/A009184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coeff of x^n/n! in cosh(x)/exp(sin(x))
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 17, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *D = NULL;   /* coefficients of exp(-sin(x)) (scaled by n!) */
static seq_t *a_arr = NULL;  /* resulting sequence a(n) */
static idx_t cap = 0;         /* current allocated size (indices 0..cap-1) */

/* resize arrays and fill missing terms up to new_cap (inclusive) */
static void ensure_cap(idx_t new_cap) {
    if (new_cap < cap) return;                /* already have enough */

    size_t new_len = new_cap + 1;

    seq_t *new_D = (seq_t*)realloc(D, new_len * sizeof(seq_t));
    seq_t *new_A = (seq_t*)realloc(a_arr, new_len * sizeof(seq_t));
    if (!new_D || !new_A) abort();            /* allocation failure */
    D = new_D;
    a_arr = new_A;

    /* compute D[n] for n >= cap */
    for (idx_t n = cap; n <= new_cap; ++n) {
        if (n == 0) { D[0] = 1; continue; }

        seq_t sum = 0;
        seq_t binom = 1;                      /* binom(n-1,0) */

        for (idx_t k = 1; k <= n; ++k) {
            if (k & 1) {                      /* G_k != 0 only for odd k */
                int half = (int)((k - 1) / 2);
                seq_t sign = (half & 1) ? 1 : -1;
                sum += sign * binom * D[n - k];
            }
            if (k < n)
                binom = binom * (seq_t)(n - k) / (seq_t)k;   /* binom(n-1,k) */
        }
        D[n] = sum;
    }

    /* compute a[n] for n >= cap using the rule:
       a[n] = sum_{k even} binom(n,k) * D[n-k]             */
    for (idx_t n = cap; n <= new_cap; ++n) {
        seq_t sum = 0;
        seq_t binom = 1;                      /* binom(n,0) */

        for (idx_t k = 0; k <= n; ++k) {
            if ((k & 1) == 0)                /* only even k */
                sum += binom * D[n - k];

            if (k < n)
                binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);   /* next binom */
        }
        a_arr[n] = sum;
    }

    cap = new_cap + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;                       /* a(0) = 1 */
    if (n >= cap) ensure_cap(n);
    return a_arr[n];
}
