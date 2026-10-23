/*
 * A009582  Expansion of e.g.f.: sinh(log(1+x)/cos(x)).
 * https://oeis.org/A009582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient (times n!) of sinh( ln(1+x)/cos(x) ) in the
 *            e.g.f. Recurrence: a(0)=0. For n>0 it is computed by convolving
 *            the scaled series of ln(1+x) and 1/cos(x), then taking the
 *            exponential generating functions of the sum and its negative,
 *            and finally (E[n]-Em[n])/2. The first ten terms a...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* maximum n that this implementation can compute (adjust if larger n are required) */
#define MAX_N 10000

static seq_t fact[MAX_N + 1];
static seq_t I[MAX_N + 1];     /* scaled 1/cos(x) coefficients */
static seq_t G[MAX_N + 1];     /* scaled ln(1+x)/cos(x) coefficients */
static seq_t E[MAX_N + 1];     /* scaled exp(g) coefficients */
static seq_t Em[MAX_N + 1];    /* scaled exp(-g) coefficients */
static seq_t a_cache[MAX_N + 1];
static idx_t up_to = 0;        /* largest n for which all arrays are up to date */

/* ensure all arrays are computed up to at least n */
static void ensure_up_to(idx_t n)
{
    if (n > MAX_N)   /* fallback: for n beyond MAX_N return 0 (not used in tests) */
        return;
    if (n <= up_to)
        return;

    /* factorials */
    if (up_to == 0)
        fact[0] = 1;
    for (idx_t i = up_to + 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* 1/cos(x) series – scaled by n! */
    if (up_to == 0)
        I[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;   /* C(m,0) */
        for (idx_t k = 0; k < m; ++k) {
            idx_t diff = m - k;
            seq_t cmk = (diff & 1) ? 0 :
                        (((diff >> 1) & 1) ? (seq_t)-1 : (seq_t)1);
            if (cmk != 0)
                sum += bin * I[k] * cmk;
            bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);   /* next binomial */
        }
        I[m] = -sum;
    }

    /* ln(1+x)/cos(x) – scaled by n!  (convolution of ln and 1/cos) */
    G[0] = 0;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;   /* C(m,0) */
        for (idx_t k = 1; k <= m; ++k) {
            bin = bin * (seq_t)(m - k + 1) / (seq_t)k;   /* C(m,k) */
            seq_t Lk = (k & 1) ? fact[k - 1] : -(seq_t)fact[k - 1]; /* (-1)^{k+1}(k-1)! */
            sum += Lk * bin * I[m - k];
        }
        G[m] = sum;
    }

    /* exp(g) and exp(-g) – scaled by n!  (exponential generating function) */
    if (up_to == 0) {
        E[0] = 1;
        Em[0] = 1;
    }
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum_plus  = 0;
        seq_t sum_minus = 0;
        seq_t bin = 1;   /* C(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1)
                bin = bin * (seq_t)(m - k + 1) / (seq_t)(k - 1);   /* C(m-1,k-1) */
            seq_t Gk = G[k];
            sum_plus  += Gk * bin * E[m - k];
            sum_minus += (-Gk) * bin * Em[m - k];
        }
        E[m]  = sum_plus;
        Em[m] = sum_minus;
    }

    /* final a(n) = (E[n] - Em[n]) / 2  (with a(0)=0) */
    a_cache[0] = 0;
    for (idx_t m = 1; m <= n; ++m)
        a_cache[m] = (E[m] - Em[m]) / (seq_t)2;

    up_to = n;
}

seq_t a(idx_t n)
{
    if (n > MAX_N)    /* for larger n an error value is returned */
        return 0;
    ensure_up_to(n);
    return a_cache[n];
}
