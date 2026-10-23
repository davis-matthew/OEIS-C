/*
 * A013123  Expansion of e.g.f. sinh(tanh(x) + log(x+1)).
 * https://oeis.org/A013123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n/n! in sinh(tanh(x)+log(1+x))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for memoisation */
static idx_t cur_max = 0;
static seq_t *fact   = NULL;
static seq_t *tanh_arr = NULL;
static seq_t *b_arr = NULL;
static seq_t *c_arr = NULL;
static seq_t *d_arr = NULL;
static seq_t *a_arr = NULL;

/* ensure all auxiliary arrays are large enough for index n */
static void ensure_size(idx_t n)
{
    if (n <= cur_max) return;

    size_t size = (size_t)(n + 1);

    fact  = (seq_t *)realloc(fact,  size * sizeof(seq_t));
    tanh_arr = (seq_t *)realloc(tanh_arr, size * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, size * sizeof(seq_t));
    c_arr = (seq_t *)realloc(c_arr, size * sizeof(seq_t));
    d_arr = (seq_t *)realloc(d_arr, size * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, size * sizeof(seq_t));

    if (!fact || !tanh_arr || !b_arr || !c_arr || !d_arr || !a_arr)
        abort();                         /* failed allocation */

    /* 1. factorials  (fact[0] = 1) */
    if (cur_max == 0) fact[0] = (seq_t)1;
    for (idx_t i = cur_max + 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* 2. tanh(x) coefficients:  t[0] = 0 initially */
    if (cur_max == 0) tanh_arr[0] = (seq_t)0;
    for (idx_t m = cur_max; m < n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t binom = fact[m] / (fact[k] * fact[m - k]);
            sum += binom * tanh_arr[k] * tanh_arr[m - k];
        }
        seq_t delta = (m == 0) ? (seq_t)1 : (seq_t)0;
        tanh_arr[m + 1] = delta - sum;
    }

    /* 3. B(x) = tanh(x) + log(1+x) */
    b_arr[0] = (seq_t)0;
    for (idx_t m = cur_max + 1; m <= n; ++m) {
        seq_t sign = (m & 1) ? (seq_t)1 : (seq_t)-1;   /* (-1)^(m-1) */
        seq_t log_part = sign * fact[m - 1];           /* (m-1)! */
        b_arr[m] = tanh_arr[m] + log_part;
    }

    /* 4. exp(B) and exp(-B) coefficients */
    c_arr[0] = (seq_t)1;
    d_arr[0] = (seq_t)1;
    for (idx_t i = cur_max; i < n; ++i) {
        seq_t sum_c = 0;
        seq_t sum_d = 0;
        for (idx_t k = 0; k <= i; ++k) {
            idx_t bidx = i - k + 1;                   /* b_{i-k+1} */
            seq_t binom = fact[i] / (fact[k] * fact[i - k]);
            sum_c += binom * c_arr[k] * b_arr[bidx];
            sum_d -= binom * d_arr[k] * b_arr[bidx];   /* minus sign for exp(-B) */
        }
        c_arr[i + 1] = sum_c;
        d_arr[i + 1] = sum_d;
    }

    /* 5. final sequence values a(n) = (c(n) - d(n)) / 2 */
    a_arr[0] = (seq_t)0;                      /* a(0) = 0 */
    for (idx_t i = cur_max; i <= n; ++i)
        a_arr[i] = (c_arr[i] - d_arr[i]) / (seq_t)2;

    cur_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_size(n);
    return a_arr[n];
}
