/*
 * A104409  Coefficients of the B-Rogers-Selberg identity.
 * https://oeis.org/A104409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Euler transform of the period‑14 sequence c =
 *            [0,0,0,1,-1,1,-1,1,-1,1,0,0,0,0] (c(1)=0, c(2)=0, c(3)=0, etc.)
 *            a(0)=1, and for n>0 a(n) = (1/n) * Σ_{k=1}^n ( Σ_{d|k} d*c(d) )
 *            * a(n-k) (Euler transform)
 * Range:     i8 n <= 17, i16 n <= 54, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_vals = NULL;      /* cache of a(n) values  */
static long long *p_vals = NULL; /* cache of divisor sums Σ_{d|k} d*c(d) */
static idx_t max_n = 0;           /* highest index computed so far        */

static const int8_t pattern[14] = {0,0,0,1,-1,1,-1,1,-1,1,0,0,0,0};

static void recompute_p(idx_t up_to) {
    /* reset all entries up to up_to */
    for (idx_t i = 1; i <= up_to; ++i)
        p_vals[i] = 0;

    /* accumulate contributions from each divisor d */
    for (idx_t d = 1; d <= up_to; ++d) {
        int8_t c = pattern[(d - 1) % 14];
        if (c != 0) {
            long long add = (long long)d * c;
            for (idx_t m = d; m <= up_to; m += d)
                p_vals[m] += add;
        }
    }
}

seq_t a(idx_t n) {
    /* initialise on first call */
    if (a_vals == NULL) {
        a_vals = (seq_t *)malloc(sizeof(seq_t));
        p_vals = (long long *)malloc(sizeof(long long));
        if (!a_vals || !p_vals) return (seq_t)0; /* out of memory */

        a_vals[0] = (seq_t)1;   /* a(0) = 1 */
        p_vals[0] = 0;          /* unused but harmless */
        max_n   = 0;
    }

    if (n == 0)
        return a_vals[0];

    /* extend computation up to n if needed */
    if (n > max_n) {
        idx_t new_max = n;

        /* reallocate storage */
        a_vals = (seq_t *)realloc(a_vals, (new_max + 1) * sizeof(seq_t));
        p_vals = (long long *)realloc(p_vals, (new_max + 1) * sizeof(long long));
        if (!a_vals || !p_vals) return (seq_t)0; /* out of memory */

        /* recompute divisor sums for all k <= new_max */
        recompute_p(new_max);

        /* compute a(k) for k = max_n+1 .. new_max */
        for (idx_t k = max_n + 1; k <= new_max; ++k) {
            seq_t acc = (seq_t)0;
            for (idx_t j = 1; j <= k; ++j)
                acc += (seq_t)p_vals[j] * a_vals[k - j];
            a_vals[k] = acc / (seq_t)k;        /* exact division */
        }

        max_n = new_max;
    }

    return a_vals[n];
}
