/*
 * A024337  Expansion of sinh(log(1+x))*log(1+x)/2.
 * https://oeis.org/A024337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * ( n! * H_n + (n-2)! ) / 4, H_n = ∑_{k=1}^n 1/k
 *            (harmonic number)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached factorials and corresponding S[n] = n! * H_n. */
static seq_t *fac = NULL;       /* fac[i] = i! */
static seq_t *S   = NULL;       /* S[i]   = i! * H_i  */
static size_t fac_len = 0;      /* number of allocated elements */

/* Ensure that fac and S contain values up to index n (inclusive). */
static void
ensure_factorial_and_S(idx_t n)
{
    /* +1 because we need to store index n inclusive */
    size_t needed = (size_t)n + 1;
    if (fac_len >= needed) {
        return;            /* already have enough */
    }

    /* Allocate or grow the arrays */
    fac = (seq_t *)realloc(fac, needed * sizeof(seq_t));
    S   = (seq_t *)realloc(S,   needed * sizeof(seq_t));

    /* Compute new entries */
    for (size_t i = fac_len; i <= n; ++i) {
        if (i == 0) {
            fac[i] = (seq_t)1;
            S[i]   = (seq_t)0;
            continue;
        }
        fac[i] = fac[i - 1] * (seq_t)i;
        if (i == 1) {
            S[i] = (seq_t)1;              /* 1! * H_1 = 1 */
            continue;
        }
        /* i >= 2 : S[i] = i * S[i-1] + fac[i-1] */
        S[i] = (seq_t)i * S[i - 1] + fac[i - 1];
    }
    fac_len = needed;
}

/* Return the nth term of the sequence. */
seq_t
a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)0;
    }
    ensure_factorial_and_S(n);

    /* term = (S[n] + fac[n-2]) / 4  with sign (-1)^n */
    seq_t term = S[n] + fac[n - 2];
    term /= (seq_t)4;
    if (n & (idx_t)1) {
        term = -term;                 /* apply (-1)^n */
    }
    return term;
}
