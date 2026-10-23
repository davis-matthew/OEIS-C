/*
 * A144377  Expansion of phi(q) / phi(q^5) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A144377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^n in phi(q)/phi(q^5), equivalently
 *            the product eta(q^2)^5 eta(q^5)^2 eta(q^20)^2 / (eta(q)^2
 *            eta(q^4)^2 eta(q^10)^5). It satisfies the recurrence (derived
 *            from the logarithmic derivative of the Euler product): a(0)=1,
 *            a(n) = (1/n) * sum_{k=1..n} S(k) * a(n-k), where S...
 * Range:     i8 n <= 15, i16 n <= 60, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>      /* size_t */

static seq_t *series = NULL;   /* a(n) values */
static size_t series_len = 0;
static seq_t *S = NULL;        /* S(k) values */
static size_t S_len = 0;

/* 20‑periodic coefficients c_k, 1‑based */
static const int c_coeff[21] = {
    0,      /* unused 0 index */
    2,   -3,   2,  -1,   0,
   -3,    2,  -1,   2,   0,
    2,   -1,   2,  -3,   0,
   -1,    2,  -3,   2,   0
};

/* linear index -> c_k value (1‑based) */
static int coeff_at(size_t d)
{
    size_t pos = d % 20;
    if (pos == 0) pos = 20;
    return c_coeff[pos];
}

static void ensure_arrays(size_t upto)
{
    /* Ensure S and series have entries up to 'upto' (inclusive) */
    if (S_len <= upto) {
        size_t new_len = upto + 1;
        S = (seq_t *)realloc(S, new_len * sizeof(seq_t));
        for (size_t k = S_len; k <= upto; ++k) {
            seq_t sum = 0;
            for (size_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    int c_d = coeff_at(d);
                    sum += (seq_t)d * (seq_t)c_d;
                    size_t d2 = k / d;
                    if (d2 != d) {
                        int c_d2 = coeff_at(d2);
                        sum += (seq_t)d2 * (seq_t)c_d2;
                    }
                }
            }
            S[k] = sum;
        }
        S_len = new_len;
    }

    if (series_len <= upto) {
        size_t new_len = upto + 1;
        series = (seq_t *)realloc(series, new_len * sizeof(seq_t));
        for (size_t n = series_len; n <= upto; ++n) {
            if (n == 0) {
                series[0] = 1;
                continue;
            }
            seq_t temp = 0;
            for (size_t k = 1; k <= n; ++k) {
                temp += S[k] * series[n - k];
            }
            series[n] = temp / (seq_t)n;  /* division is exact */
        }
        series_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have computed up to n */
    if (n >= series_len) {
        ensure_arrays((size_t)n);
    }
    return series[(size_t)n];
}
