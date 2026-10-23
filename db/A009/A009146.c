/*
 * A009146  Expansion of cosh(sin(x)*cos(x)).
 * https://oeis.org/A009146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n)! * [x^(2n)] cosh(sin(x)*cos(x))
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This expands the generating function cosh(g(x)) with g(x)=sin x cos x.
   The recurrence below computes the integer sequence a(n).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static storage for the e_m values used by the recurrence.
   e_m is defined for all m ≥ 0, and a(n) = e_{2n}. */
static seq_t *e_vals      = NULL;   /* array of e_m  */
static size_t e_cap      = 0;       /* allocated capacity   */
static size_t e_used     = 0;       /* number of elements computed (highest m + 1) */

/* Ensure that 'arr' has at least 'need' elements.
   'cap' points to the current capacity. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* Grow the e_vals array up to at least 'max_m' (inclusive). */
static void ensure_e_vals(size_t max_m)
{
    if (max_m < e_used)
        return;                         /* already computed */

    if (e_used == 0) {
        /* Allocate at least one element. */
        ensure_capacity(&e_vals, &e_cap, 1);
        e_vals[0] = (seq_t)1;          /* e_0 = 1 */
        e_used = 1;
    }

    ensure_capacity(&e_vals, &e_cap, max_m + 1);

    /* Compute e_m for m = e_used .. max_m. */
    for (size_t m = e_used; m <= max_m; ++m) {
        size_t n = m - 1;               /* n = m-1 for binomial formulas */
        seq_t sum = 0;

        /* Attributes for the current odd k. */
        seq_t comb  = (seq_t)1;          /* C(n, 0) for k = 1 (r = 0) */
        seq_t pow2  = (seq_t)1;          /* 2^(k-1) for k = 1 */
        int    sign  = 1;                /* sign for b_k */

        size_t r = 0;                    /* current r = k-1, initially 0 */

        for (size_t k = 1; k <= m; k += 2) {
            size_t prev = m - k;         /* index of e_{m-k} */

            seq_t b = pow2;
            if (sign < 0)
                b = -b;                  /* b = sign * 2^(k-1) */

            sum += comb * b * e_vals[prev];

            if (k + 2 <= m) {           /* prepare values for next odd k */
                /* Update comb: C(n, r+2) from C(n, r) */
                seq_t t1 = comb * (seq_t)(n - r);
                t1 /= (seq_t)(r + 1);

                seq_t t2 = t1 * (seq_t)(n - r - 1);
                t2 /= (seq_t)(r + 2);
                comb = t2;

                r += 2;                  /* next r = k+1 - 1 */

                /* Update pow2 and sign for next odd k: 2^(k+1-1) = 2^(k-1+2) */
                pow2 *= (seq_t)4;
                sign = -sign;            /* toggle sign */
            }
        }

        e_vals[m] = sum;
    }
    e_used = max_m + 1;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    size_t target = (size_t)n * 2;          /* a(n) = e_{2n} */
    ensure_e_vals(target);
    return e_vals[target];
}
