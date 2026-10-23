/*
 * A009655  Expansion of tan(log(1+x)^2).
 * https://oeis.org/A009655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * [x^n] tan(log(1+x)^2) (OEIS A009655)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* the build provides the types seq_t and idx_t */
static idx_t max_n = 0;                 /* biggest index pre‑computed */
static seq_t *a_vals = NULL;            /* a[k] = k! * coeff of x^k */
static seq_t *S_vals = NULL;            /* S[k] = sum_{i=0}^{k} binom(k,i) a[i] a[k-i] */
static seq_t *fact    = NULL;           /* factorials */
static seq_t *Dvals   = NULL;           /* D[m] = m! * H_m  (H_m harmonic) */
static seq_t *Cfact   = NULL;           /* Cfact[m] = m! * c_m  , c_m = 2*(-1)^{m-1} H_m */

/* grow internal tables to contain at least indices up to n */
static void grow_tables(idx_t n)
{
    if (n <= max_n) return;

    size_t newsize = (size_t)n + 1;          /* indices 0..n */
    a_vals  = (seq_t *)realloc(a_vals,  newsize * sizeof(seq_t));
    S_vals  = (seq_t *)realloc(S_vals,  newsize * sizeof(seq_t));
    fact    = (seq_t *)realloc(fact,    newsize * sizeof(seq_t));
    Dvals   = (seq_t *)realloc(Dvals,   newsize * sizeof(seq_t));
    Cfact   = (seq_t *)realloc(Cfact,   newsize * sizeof(seq_t));

    /* initialise the very first values */
    if (max_n == 0 && fact != NULL) {
        fact[0]   = (seq_t)1;
        Dvals[0]  = (seq_t)0;
        Cfact[0]  = (seq_t)0;
        a_vals[0] = (seq_t)0;
        S_vals[0] = (seq_t)0;
    }

    /* factorials */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }

    /* D[m] = m! * H_m  computed with recurrence D[m] = m*D[m-1] + fact[m-1] */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        if (m == 0) { Dvals[m] = (seq_t)0; continue; }
        Dvals[m] = (seq_t)m * Dvals[m-1] + fact[m-1];
    }

    /* Cfact[m] = 2*(-1)^{m-1} * D[m]  (Cfact[0]=0) */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        if (m == 0) { Cfact[m] = (seq_t)0; continue; }
        seq_t sign = ((m & 1ULL) ? (seq_t)1 : (seq_t)-1);
        Cfact[m] = sign * (seq_t)2 * Dvals[m];
    }

    /* compute a[k] and S[k] for new indices */
    for (idx_t m = max_n; m <= n-1; ++m) {
        /* a[m+1] */
        seq_t val = Cfact[m];          /* term m! * c_m  */
        seq_t bin = (seq_t)1;
        for (idx_t k = 0; k <= m; ++k) {
            if (k >= 1) {
                seq_t term = bin * S_vals[k] * Cfact[m-k];
                val += term;
            }
            if (k < m) {
                bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
            } else {
                bin = (seq_t)0;
            }
        }
        a_vals[m+1] = val;

        /* S[m+1] = sum_{i=0}^{m+1} binom(m+1,i) a[i] a[m+1-i] */
        seq_t Snew = (seq_t)0;
        bin = (seq_t)1;
        for (idx_t i = 0; i <= m+1; ++i) {
            Snew += bin * a_vals[i] * a_vals[m+1-i];
            if (i < m+1) {
                bin = bin * (seq_t)(m+1 - i) / (seq_t)(i + 1);
            } else {
                bin = (seq_t)0;
            }
        }
        S_vals[m+1] = Snew;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;
    grow_tables(n);
    return a_vals[n];
}
