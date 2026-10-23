/*
 * A119882  Expansion of e.g.f.: (1+x)*sech(x).
 * https://oeis.org/A119882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = EulerE[n] + n*EulerE[n-1] * (Euler numbers are defined by
 *            the e.g.f. sech(x) = Σ E_n x^n / n! ) * * Implementation
 *            computes the Euler numbers E_n via the recurrence * E_0 = 1 *
 *            E_n = -∑_{m=1}^{⌊n/2⌋} binom(n, 2m) * E_{n-2m} (n ≥ 1) * Then
 *            a(n) = E_n + n*E_{n-1} for n≥1, and a(0)=1. * The r...
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

#ifndef SEQ_TYPE_DEFINED
/* seq_t and idx_t are supplied by the build environment. */
#endif

/* Dynamic storage for Euler numbers E_n  (0 ≤ n ≤ cached_n). */
static seq_t *euler = NULL;   /* array of EulerE[0..] */
static size_t euler_len = 0;  /* number of elements currently stored   */
static size_t euler_cap = 0;  /* array capacity                         */

/* Ensure Euler numbers up to index n are computed and stored. */
static void ensure_euler(idx_t n)
{
    if (euler_len > n)
        return;

    /* Expand the array if necessary. */
    size_t required = (size_t)n + 1;
    if (required > euler_cap) {
        size_t newcap = euler_cap ? euler_cap * 2 : 16;
        if (newcap < required) newcap = required;
        euler = (seq_t *)realloc(euler, newcap * sizeof(seq_t));
        euler_cap = newcap;
    }

    /* Initialise leading values if they are missing. */
    if (euler_len == 0) {
        euler[0] = 1;
        euler_len = 1;
    }

    /* Compute missing Euler numbers using the recurrence. */
    for (size_t i = euler_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;   /* binom(i, 0) */

        /* Sum over m = 1 .. floor(i/2). */
        for (size_t m = 1; m <= i / 2; ++m) {
            /* Update binom to binom(i, 2m). */
            /* binom = binom * (i - 2m + 2) * (i - 2m + 1) /
                       ((2m) * (2m - 1)) */
            binom = binom * (seq_t)(i - 2*m + 2);
            binom = binom * (seq_t)(i - 2*m + 1);
            binom = binom / (seq_t)(2*m);
            binom = binom / (seq_t)(2*m - 1);

            /* Add contribution to the sum. */
            sum += binom * euler[i - 2*m];
        }

        euler[i] = -sum;
    }

    euler_len = required;
}

/* Public function: sequence a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 by definition */

    ensure_euler(n);               /* make sure EulerE[0..n] exist */
    seq_t nn = (seq_t)n;
    seq_t res = euler[n];
    res += nn * euler[n - 1];
    return res;
}
